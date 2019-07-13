#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"


class TCompLookAtCamera : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);

private:

};