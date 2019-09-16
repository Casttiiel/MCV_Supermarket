#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
 
class TCompFlickering :  public TCompBase {
  DECL_SIBLING_ACCESS();

  std::string target = "light";
  VEC4 constants = VEC4::Zero;

  public:
    void load(const json& j, TEntityParseContext& ctx);
    void debugInMenu();
    void renderDebug();
    void update(float delta);
};