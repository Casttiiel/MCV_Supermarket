#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
 
class TCompBoltSphere :  public TCompBase {
  DECL_SIBLING_ACCESS();

  CHandle target_position;
  CHandle target_aim;

  public:
    void load(const json& j, TEntityParseContext& ctx);
    void debugInMenu();
    void renderDebug();
    void update(float delta);
};