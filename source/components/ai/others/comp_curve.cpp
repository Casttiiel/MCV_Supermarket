#include "mcv_platform.h"
#include "comp_curve.h"
#include "components/common/comp_tags.h"


DECL_OBJ_MANAGER("comp_curve", TCompCurve);


void TCompCurve::load(const json& j, TEntityParseContext& ctx) {

	if (j.count("curve") > 0) {
		_curve = Resources.get(j.value("curve", ""))->as<CCurve>();
	}


	_knots = _curve->_knots;
}

std::vector<VEC3> TCompCurve::getCurve(){

	return _knots;
}


void TCompCurve::update(float delta) {

}

