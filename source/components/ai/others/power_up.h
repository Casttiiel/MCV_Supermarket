#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_collider.h"
#include "components/common/comp_transform.h"


class TCompPowerUp : public TCompBase {


	VEC3  front;
	CHandle h_sender;
	
	bool destroy = false;
	int power_type = 1; //1 banana 2 grenade 3 battery


	DECL_SIBLING_ACCESS();
	void onCollision(const TMsgOnContact& msg);

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);

	static void registerMsgs();

};