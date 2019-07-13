#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_collider.h"

class TCompExplodeOnContact : public TCompBase {

	
	DECL_SIBLING_ACCESS();

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();

private:
	float _explosionForce = 10.f;
	float _explosionDamage = 20.f;
	float _explosionRadius = 6.f;

	bool _collided = false;

  void onCollision(const TMsgOnContact& msg);
};