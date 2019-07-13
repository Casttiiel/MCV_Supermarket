#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
 
class TCompScalable :  public TCompBase {
  DECL_SIBLING_ACCESS();

  float initial_scale = 1.0f;
  float final_scale = 1.0f;
  float time_to_scale = 1.0f;
  float speed = 0.0f;
  void onCreation(const TMsgEntityCreated& msg);

  public:
    void load(const json& j, TEntityParseContext& ctx);
    void debugInMenu();
    void renderDebug();
    void update(float delta);
    static void registerMsgs();
};