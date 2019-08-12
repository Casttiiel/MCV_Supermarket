#include "mcv_platform.h"
#include "comp_grenade_golem.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_tags.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"
#include "modules/module_physics.h"
#include "components/common/comp_name.h"

#include "PxPhysicsAPI.h"

using namespace physx;

DECL_OBJ_MANAGER("grenade_golem_controller", TCompGrenadeGolemController);

void TCompGrenadeGolemController::debugInMenu() {
	

	
}


void TCompGrenadeGolemController::load(const json& j, TEntityParseContext& ctx) {
	//init_time_explosion = j.value("speed_explosion", init_time_explosion);
	explosion_radius = j.value("explosion_radius", explosion_radius);
}

void TCompGrenadeGolemController::registerMsgs() {
	DECL_MSG(TCompGrenadeGolemController, TMsgAssignBulletOwner, onGrenadeInfoMsg);
	DECL_MSG(TCompGrenadeGolemController, TMsgOnContact, onCollision);
}

void TCompGrenadeGolemController::onCollision(const TMsgOnContact& msg) {
	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
  TCompCollider* c_col = source_of_impact->get<TCompCollider>();

  dbg("el proyectil cupcake detecta la colision con el suelo - - - - - - - - - - - - - - - -\n");

  PxShape* colShape;
  c_col->actor->getShapes(&colShape, 1, 0);
  PxFilterData col_filter_data = colShape->getSimulationFilterData();
  if (col_filter_data.word0 & EnginePhysics.Scenario) {
    flagExplota = true;
  }
	
}


void TCompGrenadeGolemController::onGrenadeInfoMsg(const TMsgAssignBulletOwner& msg) {
	h_sender = msg.h_owner;
	c_trans = get<TCompTransform>();
	c_trans->setPosition(msg.source);
	float yaw = vectorToYaw(msg.front);
	c_trans->setAngles(yaw, 0.f, 0.f);
	dbg("Naces en x:%f,y:%f,z:%f\n", c_trans->getPosition().x, c_trans->getPosition().y, c_trans->getPosition().z);
	
	c_collider = get<TCompCollider>();
	VEC3 new_pos = c_trans->getFront();
	PxVec3 velocidad = PxVec3(new_pos.x, new_pos.y, new_pos.z);

	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_collider->actor);
	rigid_dynamic->setMass(0.75);
	PxVec3 vel_ang = PxVec3(1,0,0);
	rigid_dynamic->setAngularVelocity(vel_ang);
	rigid_dynamic->addForce(velocidad * 10 , PxForceMode::eIMPULSE, false) ;
	PxVec3 velocidady = PxVec3(0, new_pos.y+8, 0);
	rigid_dynamic->addForce(velocidady, PxForceMode::eIMPULSE, false);
	
}

void TCompGrenadeGolemController::update(float delta) {//cambiar la condicion un poco antes de la explosion para indicar que va a explotar
	PROFILE_FUNCTION("GrenadeGolem");
	if (flagExplota) {
		TCompTransform* c_trans = get<TCompTransform>();
		dbg("spawn cupcake");
		TEntityParseContext ctx;
		ctx.root_transform.setPosition(c_trans->getPosition());
		parseScene("data/prefabs/enemies/bt_cupcake.json", ctx);

		//Enviar mensaje con la posicion de la granada para saber si le das o no
		CHandle(this).getOwner().destroy();
	}
}

	

