#include "mcv_platform.h"
#include "comp_follow.h"
#include "components/common/comp_transform.h"
#include "components/common/physics/comp_collider.h"
#include "engine.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_follow", TCompFollow);

void TCompFollow::debugInMenu() {
}

void TCompFollow::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", speed);
	minDistance = j.value("minDistance", minDistance);
	targetName = j.value("targetName", "");
	h_target = ctx.findEntityByName(targetName);
}

void TCompFollow::update(float dt) {
	if (!h_target.isValid()) return;
	
	TCompTransform* trans = get<TCompTransform>();
	CEntity* eTarget = h_target;
	TCompTransform* posTarget = eTarget->get<TCompTransform>();
	
	TCompCollider* col = get<TCompCollider>();
	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(col->actor);
	VEC3 dir = posTarget->getPosition() - trans->getPosition();
	dir.Normalize();
	VEC3 nextPos = trans->getPosition() + dir * speed * dt;
	QUAT quat = trans->getRotation();
	PxVec3 pos = VEC3_TO_PXVEC3(nextPos);
	PxQuat qua = QUAT_TO_PXQUAT(quat);
	const PxTransform tr(pos, qua);
	rigid_dynamic->setKinematicTarget(tr);
}