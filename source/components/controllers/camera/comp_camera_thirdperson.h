#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"


class TCompCameraThirdPerson : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void setTimeScale(float timeScale);
  float getTimeScale();

  float _timeScale = 1.0f;

private:
  float distance_ = 5.5f;
  float distance_2 = 4.2f;
  float distance_to_player = distance_;
  float sensitivity_x = 0.5f;
  float sensitivity_y = 0.5f;
  float rotation_speed = 5.0f;
  float enclosing_speed = 17.0f;
  float yaw_rotation_speed = 0.0f;
  float pitch_rotation_speed = 0.0f;
  float decreasing_factor = 0.8f;
  
  float angle_min_y = 0.0f;
  float angle_max_y = 1.25f;
  
 
  int   _key_toggle_enabled = 0;
  bool  _enabled = true;
  float yaw = 0.f;
  float pitch = 0.f;
  VEC3 offset = VEC3(0, 2.5f, 0);

  CHandle h_player;
	int camera_type = 0;
};

