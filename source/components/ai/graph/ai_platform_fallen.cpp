#include "mcv_platform.h"
#include "ai_platform_fallen.h"
#include "engine.h"
#include "entity/entity.h"
#include "components/common/comp_transform.h"
#include "components/common/physics/comp_collider.h"
#include "components/common/comp_render.h"
#include "components/common/comp_tags.h"
#include "time.h"
#include "stdlib.h"
#include "modules/module_physics.h"
#include "ai_controller.h"


#include "PxPhysicsAPI.h"


using namespace physx;


DECL_OBJ_MANAGER("ai_platform_fallen", CAIFallenPlatform);

void CAIFallenPlatform::Init()
{
	AddState("IDLE", (statehandler)&CAIFallenPlatform::IdleState);
	AddState("DEAD", (statehandler)&CAIFallenPlatform::DeadState);

	ChangeState("IDLE");
}

void CAIFallenPlatform::IdleState(float dt) {
	if (flagfallen) {
		if (timeRecoveryAux > 0) {
			timeRecoveryAux -= dt;
		}
		else {
			timeRecoveryAux = timeRecovery;
			TCompCollider* c_col = get<TCompCollider>();
			physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
			rigid_dynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			
			const PxTransform tr(VEC3_TO_PXVEC3(pos));
			rigid_dynamic->setKinematicTarget(tr);
			flagfallen = false;
		}
		
	}
	
}

void CAIFallenPlatform::DeadState(float dt) {
	TCompCollider* c_col = get<TCompCollider>();
	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
	rigid_dynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//PxVec3 velocidad = PxVec3(0, -10, 0);//poner un tiempo y que se caiga
	//rigid_dynamic->addForce(velocidad, PxForceMode::eACCELERATION, false);
	if (timeFallenAux > 0) {
		timeFallenAux -= dt;
	}
	else {
		PxVec3 velocidad = PxVec3(0, -100, 0);//poner un tiempo y que se caiga
	    rigid_dynamic->addForce(velocidad, PxForceMode::eACCELERATION, false);
		flagfallen = true;
		timeFallenAux = timeFallen;
		ChangeState("IDLE");
	}

	
	
}


void CAIFallenPlatform::onCollision(const TMsgOnContact& msg) {
	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
 	if (source_of_impact) {
		TCompTags* c_tag = source_of_impact->get<TCompTags>();
		if (c_tag) {
			std::string tag = CTagsManager::get().getTagName(c_tag->tags[0]);
			if (strcmp("player", tag.c_str()) == 0) {
				ChangeState("DEAD");
			}

		}
	}
}


void CAIFallenPlatform::registerMsgs() {
	DECL_MSG(CAIFallenPlatform, TMsgOnContact, onCollision);
}

void CAIFallenPlatform::load(const json& j, TEntityParseContext& ctx) {
	
	pos = loadVEC3("pos");
	timeFallen = j.value("timeFallen", timeFallen);
	timeFallenAux = timeFallen;
	this->Init();
}

void CAIFallenPlatform::debugInMenu() {
	
}

void CAIFallenPlatform::renderDebug() {
	
}



