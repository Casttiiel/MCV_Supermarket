#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "geometry/curve.h"


class TCompCurve : public TCompBase {
	DECL_SIBLING_ACCESS();

public:



	void load(const json& j, TEntityParseContext& ctx);

	void update(float delta);
	
	std::vector<VEC3> getCurve();



private:

	std::vector<VEC3> _knots;
	const CCurve* _curve = nullptr;
};