#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_rigid_body.h"
#include "components/common/comp_transform.h"
class TCompWindTrap : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();

private:
	bool _isEnabled = false;
	float windForce = 50.f;

	float _windDuration = 3.f;
	float _windCurrentTime = 0.f;

	float _windDelay = 2.f;
	float _windCooldownTimer = _windDelay;
	CHandle player;

	TCompTransform* c_trans;

	void enable(const TMsgEntityTriggerEnter & msg);
	void disable(const TMsgEntityTriggerExit & msg);
	void onBattery(const TMsgGravity & msg);
	void onCreate(const TMsgEntityCreated & msg);
};

