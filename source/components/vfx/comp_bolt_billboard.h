#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
 
class TCompBoltBillboard :  public TCompBase {
  DECL_SIBLING_ACCESS();

  CHandle target_position;
  CHandle target_aim;
  float prev_scale = 0.0f;
  VEC4 constants = VEC4::Zero;

  public:
    void load(const json& j, TEntityParseContext& ctx);
    void debugInMenu();
    void renderDebug();
    void update(float delta);
};