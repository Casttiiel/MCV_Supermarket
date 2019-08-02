#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"


class TCompFollow : public TCompBase {

	
	DECL_SIBLING_ACCESS();

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();

private:
	float speed = 2.0f;
	float minDistance = 3.f;
	CHandle     h_target;
	std::string targetName = "";
};