#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"

class TCompCameraLookat : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

private:
  CHandle _target;
  std::string _targetName;
};

