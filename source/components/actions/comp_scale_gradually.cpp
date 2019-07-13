#include "mcv_platform.h"
#include "comp_scale_gradually.h"
#include "components/common/comp_transform.h"
#include "engine.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_scale_gradually", TCompScaleGradually);

void TCompScaleGradually::debugInMenu() {
}

void TCompScaleGradually::load(const json& j, TEntityParseContext& ctx) {
    _finalScale = j.value("_finalScale", _finalScale);
    _ratio = j.value("_ratio", _ratio);
    _unscaledTime = j.value("_unscaledTime", _unscaledTime);
}

void TCompScaleGradually::registerMsgs() {
}

void TCompScaleGradually::update(float dt) {
    //Scale transform
    TCompTransform* c_trans = get<TCompTransform>();
    TCompCollider* c_col = get<TCompCollider>();
    if (c_trans && c_col) {
        if (c_trans->getScale() < _finalScale) {
            float currentScale = c_trans->getScale();
            float newScale = currentScale + Time.delta_unscaled;
            c_trans->setScale(newScale);
            //Scale collider
            PxSphereGeometry sphere;
            PxShape* colShape;
            c_col->actor->getShapes(&colShape, 1, 0);
            colShape->getSphereGeometry(sphere);
            sphere.radius = currentScale + Time.delta_unscaled;
        }
    }
}