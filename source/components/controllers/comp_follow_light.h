#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"

class TCompFollowLight : public TCompBase {

  VEC3 offset;
  CHandle e_player;

public:
  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  DECL_SIBLING_ACCESS();


};