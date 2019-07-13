#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_collider.h"
#include "components/common/comp_transform.h"


class TCompDeath : public TCompBase {

	int damage = 999;
	float damageTimer = 0.0f;
	float damageInterval = 1.0f;
	bool collision = false;
	CEntity* impactElement;

	DECL_SIBLING_ACCESS();
	void onCollision(const TMsgOnContact& msg);

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);

	static void registerMsgs();

};