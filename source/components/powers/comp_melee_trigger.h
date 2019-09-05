#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_collider.h"
#include "components/common/comp_transform.h"
class TCompMeleeTrigger : public TCompBase {


	DECL_SIBLING_ACCESS();
	void onCollision(const TMsgEntityTriggerEnter& msg);


public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);
	void renderDebug();
	void debugInMenu();
	static void registerMsgs();

private:
    bool _isEnabled = true;
	TMsgDamage _messageToTarget;
    EntityType targetType;
    std::string _audioOnHit = "event:/Character/Attacks/Melee_Hit";
};

