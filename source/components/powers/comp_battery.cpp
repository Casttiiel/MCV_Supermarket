#include "mcv_platform.h"
#include "comp_battery.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_tags.h"
#include "components/common/physics/comp_collider.h"
#include "input/module_input.h"
#include "modules/module_physics.h"
#include "components/controllers/comp_parabolic_launch.h"

using namespace physx;

DECL_OBJ_MANAGER("battery_controller", TCompBatteryController);
PxVec3 vector = PxVec3(1, 2, 3);
void TCompBatteryController::debugInMenu() {
    ImGui::DragFloat("Force", &force, 0.1f, 0.0f, 20.0f);
    ImGui::DragFloat("Height Force", &height_force, 0.1f, 0.0f, 20.0f);
    ImGui::DragFloat("Time effect", &timeEffect, 0.1f, 0.0f, 20.0f);
}

void TCompBatteryController::load(const json& j, TEntityParseContext& ctx) {
    battery_radius_hit = j.value("battery_radius_hit", battery_radius_hit);
    mass = j.value("mass", mass);
    timeEffect = j.value("timeEffect", timeEffect);
    force = j.value("force", force);
    height_force = j.value("height_force", height_force);
    atractionForce = j.value("atraction_force", height_force);
    pulseDelay = j.value("pulseDelay", pulseDelay);
    maxEnemiesAffected = j.value("maxEnemiesAffected", maxEnemiesAffected);
}

void TCompBatteryController::registerMsgs() {
    DECL_MSG(TCompBatteryController, TMsgAssignBulletOwner, onBatteryInfoMsg);
    DECL_MSG(TCompBatteryController, TMsgOnContact, onCollision);
    DECL_MSG(TCompBatteryController, TMsgDamage, onBatteryFire);
}

void TCompBatteryController::onBatteryFire(const TMsgDamage & msg) {
    //Creacion de fuego y dano a enemigos que esten aqui
    if (msg.damageType == FIRE) {
        batteryFire = true; //esto se enviara en el mensaje que le envia al enemy
        //spawn here only once an explosion prefab with pulseTimer left of destruction
    }

}
void TCompBatteryController::onCollision(const TMsgOnContact& msg) {

    CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
    if (source_of_impact) {
        PxShape* colShape;
        TCompCollider* c_collider = get<TCompCollider>();
        if (c_collider->actor == nullptr) return;
        c_collider->actor->getShapes(&colShape, 1, 0);
        PxFilterData col_filter_data = colShape->getSimulationFilterData();



        if (col_filter_data.word0 & EnginePhysics.Scenario) {
            isKinematic = false;

            physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_collider->actor);

            //spawn here the bolt sphere
            //spawn a bolt from the sphere to this USING LUA

        }
        //antes 
          //rigid_dynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
    }
}


void TCompBatteryController::onBatteryInfoMsg(const TMsgAssignBulletOwner& msg) {

    h_sender = msg.h_owner;
    c_trans = get<TCompTransform>();

    TCompCollider* c_collider = get<TCompCollider>();
    physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_collider->actor);
    rigid_dynamic->setMass(1000);
    //VEC3 new_pos = c_trans->getFront();
    CEntity* e_camera = getEntityByName("PlayerCamera");
    TCompParabolicLaunch* parabolicLaunch = e_camera->get<TCompParabolicLaunch>();
    CCurve* curve_dynamic = parabolicLaunch->curve_dynamic;
    _knots = curve_dynamic->getKnots();
    nextPoint = _knots[i];
    c_trans->setPosition(nextPoint);
    audioEffect = EngineAudio.playEvent("event:/Character/Powers/Battery/Battery");

}

void TCompBatteryController::update(float delta) {
    PROFILE_FUNCTION("BatteryBullet");
    TCompTransform* p_trans = ((CEntity*)(GameController.getPlayerHandle()))->get<TCompTransform>();
    audioEffect.set3DAttributes(*p_trans);

    if (isKinematic) { //FOLLOW CURVE
        TCompTransform* c_trans = get<TCompTransform>();
        TCompCollider* c_col = get<TCompCollider>();
        physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);

        VEC3 currentPosition = c_trans->getPosition();
        VEC3 direccion = nextPoint - currentPosition;
        direccion.Normalize();
        Vector3 nextPos = currentPosition + direccion * velocityCurve * delta;
        //dbg("NextPos: %f,%f,%f\n", nextPos.x, nextPos.y, nextPos.z);
        PxVec3 pos = VEC3_TO_PXVEC3(nextPos);
        const PxTransform tr(pos);
        rigid_dynamic->setGlobalPose(tr, false);
        //rigid_dynamic->setKinematicTarget(tr);
        float dist = Vector3::Distance(nextPoint, c_trans->getPosition());
        if (dist < 1.5f) {
            i++;

            if (i < 99) {
                nextPoint = _knots[i];
            }
        }
    }
    else { //LA PILA COLISIONA
        if (pulseTimer <= 0.f){
            //Remove non valid enemies from the array
            std::list<CHandle>::iterator i = enemiesCatchBattery.begin();
            while (i != enemiesCatchBattery.end())
            {
                if (!(*i).isValid())
                {
                    enemiesCatchBattery.erase(i++);  // alternatively, i = items.erase(i);
                }
                else
                {
                    ++i;
                }
            }
            if (enemiesCatchBattery.size() < maxEnemiesAffected) {
                //RADIO DE GRAVEDAD
                TCompTransform* c_trans = get<TCompTransform>();
                PxSphereGeometry geometry(battery_radius_hit);
                Vector3 pos = c_trans->getPosition() + (Vector3().Up * gravityOriginHeight);
                PxQuat ori = QUAT_TO_PXQUAT(c_trans->getRotation());

                const PxU32 bufferSize = 256;
                PxOverlapHit hitBuffer[bufferSize];
                PxOverlapBuffer buf(hitBuffer, bufferSize);
                PxTransform shapePose = PxTransform(VEC3_TO_PXVEC3(pos), ori);

                PxQueryFilterData filter_data = PxQueryFilterData();
                filter_data.data.word0 = EnginePhysics.Enemy;

                bool res = EnginePhysics.gScene->overlap(geometry, shapePose, buf, filter_data);
                if (res) {
                    for (PxU32 i = 0; i < buf.nbTouches; i++) {
                        CHandle h_comp_physics;
                        h_comp_physics.fromVoidPtr(buf.getAnyHit(i).actor->userData);
                        CEntity* entityContact = h_comp_physics.getOwner();
                        if (entityContact && enemiesCatchBattery.size() < maxEnemiesAffected) {
                            TMsgGravity msg;
                            msg.h_sender = h_sender;      // Who send this bullet
                            msg.h_bullet = CHandle(this).getOwner(); // The bullet information
                            msg.position = PXVEC3_TO_VEC3(pos);
                            msg.time_effect = timeEffect;
                            msg.distance = distance;
                            msg.attractionForce = atractionForce;
                            CEntity* entityEnemyDamage = entityContact;
                            //dbg("Entity Enemy %s\n", entityEnemyDamage->getName());
                            enemiesCatchBattery.push_back(h_comp_physics);
                            entityEnemyDamage->sendMsg(msg);

                            //spawn here a bolt from this to the enemy

                            //TODO: si fire esta activado enviar mensaje de damage cada x tiempo
                            if (batteryFire) {
                                if (fireTime <= 0) {
                                    fireTime = fireTimeMax;
                                    TMsgDamageToAll msgDamage;
                                    //msgDamage.intensityDamage = fireDamage;
                                    //entityEnemyDamage->sendMsg(msgDamage);
                                    for (auto&& enemy : enemiesCatchBattery) {
                                        msgDamage.intensityDamage = fireDamage;
                                        CEntity* enemyCaught = enemy;
                                        enemyCaught->sendMsg(msgDamage);
                                        //dbg("Entity Enemy %s\n", enemy->getName());
                                    }
                                    //dbg("Entity Enemy %s\n",entityEnemyDamage->getName());
                                    //dbg("damage de fuego desde la pila enviado al enemigo \n");
                                }
                                else {
                                    fireTime -= delta;
                                }
                            }

                        }
                    }
                    if (timeEffect > 0) {
                        timeEffect -= delta;
                    }
                    else {
                        timeEffect = 0;
                        batteryFire = false;
                    }
                }
                pulseTimer = pulseDelay;
                dbg("Battery pulses\n");
            }
        }
        else {
            pulseTimer -= delta;
        }
    }

    //DESTROY WHEN IT HAS ENDED
    if (limitTime > 0.f) {
        limitTime -= delta;
    }
    else {
        TMsgBatteryDeactivates msg;
        ((CEntity*)GameController.getPlayerHandle())->sendMsg(msg);
        audioEffect.stop();
        CHandle(this).getOwner().destroy();
        CHandle(this).destroy();
    }
}

void TCompBatteryController::renderDebug() {
    TCompTransform* c_trans = get<TCompTransform>();
    Vector3 front = c_trans->getFront();
    Vector3 pos = c_trans->getPosition();
    if (timeEffect > 0 && flagColisiona) {
        TCompTransform* c_trans = get<TCompTransform>();
        Vector3 gravityOrigin = c_trans->getPosition() + (Vector3().Up * gravityOriginHeight);
        drawWiredSphere(gravityOrigin, battery_radius_hit, VEC4(1, 0, 0, 1));
    }
}

void TCompBatteryController::shoot(VEC3 front) {
    TEntityParseContext ctx;
    CHandle h_player = getEntityByName("Player");
    CEntity* e_player = (CEntity *)h_player;
    TCompTransform* c_trans = e_player->get<TCompTransform>();
    ctx.root_transform = *c_trans;
    parseScene("data/prefabs/bullets/battery.json", ctx);
    TMsgAssignBulletOwner msg;
    msg.h_owner = CHandle(this).getOwner();
    msg.source = c_trans->getPosition();
    msg.front = front;
    ctx.entities_loaded[0].sendMsg(msg);
}