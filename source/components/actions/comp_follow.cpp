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
  if (!h_target.isValid())
    h_target = getEntityByName(targetName);
}

void TCompFollow::update(float dt) {
	if (!h_target.isValid()) return;
	
	 
	TCompTransform* trans = get<TCompTransform>();
	CEntity* eTarget = h_target;
	TCompTransform* posTarget = eTarget->get<TCompTransform>();
	if (minDistance <= Vector3::Distance(posTarget->getPosition(), trans->getPosition())) {
		VEC3 dir = posTarget->getPosition() - trans->getPosition();
		dir.Normalize();
		VEC3 nextPos = trans->getPosition() + dir * speed * dt;
    trans->setPosition(nextPos);
	}
}