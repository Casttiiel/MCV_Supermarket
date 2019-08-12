#pragma once

#include "comp_base.h"
 
struct TCompNumInstances : public TCompBase {
  uint32_t num_instances;
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
};