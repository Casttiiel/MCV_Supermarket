#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_rigid_body.h"
#include "components/common/comp_transform.h"
#include "modules/game/audio/audioEvent.h"
class TCompWindTrap : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  bool canBeDestroyed(VEC3 battery_pos);
  void enableWind() { activateWind = true; }

  static void registerMsgs();

private:
    AudioEvent audio;
  bool activateWind = true;
	bool _isEnabled = false;
  bool is_destroyed = false;
	float windForce = 50.f;
  
  float windLength = 0.6f;
  float windDist = 0.1f;

	float _windDuration = 3.f;
	float _windCurrentTime = 0.f;

	float _windDelay = 0.16f;
	float _windCooldownTimer = 0.0f;
  float _scaleVar = 0.7f;
  float _rollVar = 1.57f;
  float _speedVar = 2.0f;
  float _radius = 0.6f;
  float _startDestroy = 3.0f;
  float _destroyVar = 0.8f;
  float distanceThreshold = 8.f;

	CHandle player;

	TCompTransform* c_trans;

	void enable(const TMsgEntityTriggerEnter & msg);
	void disable(const TMsgEntityTriggerExit & msg);
	void onBattery(const TMsgGravity & msg);
	void onCreate(const TMsgEntityCreated & msg);
  void generateWind(float dt);
};

