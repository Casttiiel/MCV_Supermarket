#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"


class TCompRotator : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

private:
	float rotationTime = 0.f;
	VEC3 axis = VEC3(0, 1, 0);
	bool activateRotator = false;
};

