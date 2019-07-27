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
  
  float windLength = 0.8f;
  float windDist = 0.4f;

	float _windDuration = 3.f;
	float _windCurrentTime = 0.f;

	float _windDelay = 0.08f;
	float _windCooldownTimer = 0.0f;
  float _scaleVar = 0.4f;
  float _rollVar = 1.57f;
  float _speedVar = 2.0f;
  float radius = 0.5f;

	CHandle player;

	TCompTransform* c_trans;

	void enable(const TMsgEntityTriggerEnter & msg);
	void disable(const TMsgEntityTriggerExit & msg);
	void onBattery(const TMsgGravity & msg);
	void onCreate(const TMsgEntityCreated & msg);
  void generateWind(float dt);
};

