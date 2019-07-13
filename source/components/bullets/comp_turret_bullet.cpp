#include "mcv_platform.h"
#include "comp_turret_bullet.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_tags.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"
#include "modules/module_physics.h"

using namespace physx;

DECL_OBJ_MANAGER("turret_bullet_controller", TCompTurretBulletController);

void TCompTurretBulletController::debugInMenu() {
  ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 500.0f);
  ImGui::DragFloat("Damage", &intensityDamage, 0.1f, 0.0f, 100.0f);
}

void TCompTurretBulletController::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", speed);
	intensityDamage = j.value("damage", intensityDamage);
	mass = j.value("mass", mass);
}

void TCompTurretBulletController::registerMsgs() {
	DECL_MSG(TCompTurretBulletController, TMsgAssignBulletOwner, onBulletInfoMsg);
  DECL_MSG(TCompTurretBulletController, TMsgOnContact, onCollision);
}

void TCompTurretBulletController::onCollision(const TMsgOnContact& msg) {

	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
	TCompCollider* c_col = source_of_impact->get<TCompCollider>();

	PxShape* colShape;
	c_col->actor->getShapes(&colShape, 1, 0);
	PxFilterData col_filter_data = colShape->getSimulationFilterData();
	if (col_filter_data.word0 & EnginePhysics.Scenario || col_filter_data.word0 & EnginePhysics.Player) {
		explode();
	}
}

void TCompTurretBulletController::explode() {
	dbg("Eldumplig explota .\n");
	//Generate damage sphere
	TCompTransform* c_trans = get<TCompTransform>();
	TMsgDamage msg;
	msg.senderType = EntityType::ENVIRONMENT;
	msg.targetType = EntityType::ALL;
	msg.position = c_trans->getPosition();
	msg.intensityDamage = intensityDamage;
	msg.impactForce = _explosionForce;
	GameController.generateDamageSphere(c_trans->getPosition(), _explosionRadius, msg, "VulnerableToExplosions");
	//Get rid of the object's collider (the shape)
	TCompCollider* c_col = get<TCompCollider>();
	static const physx::PxU32 max_shapes = 8;
	physx::PxShape* shapes[max_shapes];
	const physx::PxU32 bufferSize = 256;
	c_col->actor->getShapes(shapes, bufferSize);
	physx::PxShape* sh = shapes[0];
	//c_col->actor->detachShape(*sh); //comento para que no se vaya por debajo del suelo

	//_isFireEnabled = true;
	//TCompRender* crender = get<TCompRender>();
	//crender->showMeshesWithState(1);

	//_hasExploded = true;
	GameController.spawnPrefab("data/prefabs/vfx/explosion.json", c_trans->getPosition());
	impact = true;
}

void TCompTurretBulletController::onBulletInfoMsg(const TMsgAssignBulletOwner& msg) {
	
	h_sender = msg.h_owner;
	TCompTransform* c_trans = get<TCompTransform>();
	c_trans = get<TCompTransform>();
	c_trans->setPosition(msg.source);
	float yaw = vectorToYaw(msg.front);
	c_trans->setAngles(yaw, 0.f, 0.f);

	TCompCollider* c_collider = get<TCompCollider>();
	c_collider = get<TCompCollider>();
	VEC3 new_pos = c_trans->getFront();
	PxVec3 velocidad = PxVec3(new_pos.x, new_pos.y, new_pos.z);

	CHandle h_player = GameController.getPlayerHandle();
	CEntity* e_player = (CEntity*)h_player;
	TCompTransform* player_trans = e_player->get< TCompTransform>();

	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_collider->actor);
	rigid_dynamic->setMass(mass);
	PxVec3 vel_ang = PxVec3(1, 0, 0);
	rigid_dynamic->setAngularVelocity(vel_ang);

	float impulse_front = 1.2;

	impulse_front *= VEC3().Distance(player_trans->getPosition(), c_trans->getPosition());

	float impulso_y = 10;


	rigid_dynamic->addForce(velocidad * impulse_front, PxForceMode::eIMPULSE, false); //todo
	PxVec3 velocidady = PxVec3(0, new_pos.y + impulso_y, 0);//todo
	rigid_dynamic->addForce(velocidady, PxForceMode::eIMPULSE, false);
	
	float angle = rad2deg(atan((0.1125)));//atan pasa radianes




	/*
	CHandle h_player = getEntityByName("Player");
	if (!h_player.isValid()) {
		e_player = getEntityByName("Player");
		h_player = ((CEntity*)e_player)->get<TCompTransform>();
		return;
	}
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform *transfPlayer = e_player->get<TCompTransform>();
	float distance = VEC3::Distance(transfPlayer->getPosition(), c_trans->getPosition());
	float g = 9.8;
	float tanAlfa = tanf(deg2rad(70));
	float h = 1;//fabs(transfPlayer->getPosition().y - c_trans->getPosition().y);

	float vz = sqrt(g * distance * distance / (2.0f * (fabs(h - distance * tanAlfa))));
	float vy = tanAlfa * vz;
	PxVec3 velz = PxVec3(0, 0, vz);
	PxVec3 vely = PxVec3(0, vy, 0);

	PxVec3 vel_ang = PxVec3(1, 0, 0);
	rigid_dynamic->setAngularVelocity(vel_ang);
	rigid_dynamic->addForce(velz, PxForceMode::eIMPULSE, false);
	PxVec3 velocidady = PxVec3(0, vy, 0);
	rigid_dynamic->addForce(velocidady, PxForceMode::eIMPULSE, false);
	*/
}

void TCompTurretBulletController::update(float delta) {
	PROFILE_FUNCTION("TurretBullet");
	if (life_time <= 0 || impact) {
		CHandle(this).getOwner().destroy();
	}
	else {
		life_time -= delta;
	}

	
	 
}


