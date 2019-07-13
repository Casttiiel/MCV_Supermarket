#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/comp_buffers.h" 


class TCompCircularController : public TCompBase {

  float speed = 1.0f;
  float radius = 1.0f;
  float current_yaw = 0.f;
  VEC3  center;

  DECL_SIBLING_ACCESS();
  void onEntityCreated(const TMsgEntityCreated& msg);

  struct TMorphWeights {
    float a = 0, b = 0, c = 0, d = 0;
  };
  TMorphWeights morph_weights;

public:
  void update(float dt, uint32_t idx);
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();
  static void registerMsgs();
};