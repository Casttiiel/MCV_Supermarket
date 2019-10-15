#include "mcv_platform.h"
#include "engine.h"
#include "comp_balance.h"
#include "components/common/comp_tags.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "modules/module_physics.h"
#include "components/common/comp_name.h"
using namespace physx;
std::mt19937 ba_mt_ba(std::random_device{}());
std::uniform_int_distribution<int> ba_mt_ba_dist(-4, 4);


DECL_OBJ_MANAGER("comp_balance", TCompBalance);

void TCompBalance::debugInMenu() {
}

void TCompBalance::load(const json& j, TEntityParseContext& ctx) {
	

}

void TCompBalance::registerMsgs() {
	DECL_MSG(TCompBalance, TMsgEntityCreated, onCreate);
}

void TCompBalance::onCreate(const TMsgEntityCreated & msg) {
	TCompCollider* c_col = get<TCompCollider>();
	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
	//rigid_dynamic->setLinearVelocity(PxVec3(0, 100000,0));

	float velocitiRandom = ba_mt_ba_dist(ba_mt_ba);
  rigid_dynamic->addForce(PxVec3(0, velocitiRandom, 0), PxForceMode::eVELOCITY_CHANGE);
	
}

void TCompBalance::balanceo() {
	/*TCompCollider* c_col = get<TCompCollider>();
	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
	float velocitiRandom = ba_mt_ba_dist(ba_mt_ba);
	rigid_dynamic->addForce(PxVec3(0, -900, 0), PxForceMode::eFORCE);
	*/
	/*VHandles v_tp_joints = CTagsManager::get().getAllEntitiesByTag(getID("joints"));
	for (const auto& entity : v_tp_joints) {
		CEntity* e_entity = (CEntity*)entity;
		TCompCollider* comp_balance = e_entity->get<TCompCollider>();
		TCompName* comp_name = e_entity->get<TCompName>();
		dbg("%s\n",comp_name->getName());
		physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
		rigid_dynamic->addForce(PxVec3(0, -900, 0), PxForceMode::eVELOCITY_CHANGE);
	}*/
	getObjectManager<TCompBalance>()->forEach([](TCompBalance* di) {
		TCompCollider* c_col = di->get<TCompCollider>();
		physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
		float velocitiRandom = ba_mt_ba_dist(ba_mt_ba);
		rigid_dynamic->addForce(PxVec3(0, -900, 0), PxForceMode::eFORCE);
		
	});
	
}


void TCompBalance::update(float dt) {

}

