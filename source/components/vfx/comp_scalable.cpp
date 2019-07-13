#include "mcv_platform.h"
#include "comp_scalable.h"
#include "components/common/comp_transform.h"

DECL_OBJ_MANAGER("scalable", TCompScalable);

void TCompScalable::debugInMenu() {

}

void TCompScalable::load(const json& j, TEntityParseContext& ctx) {
  //get final scale and time
  final_scale = j.value("final_scale", final_scale);
  time_to_scale = j.value("time_to_scale", time_to_scale);
}

void TCompScalable::registerMsgs() {
  DECL_MSG(TCompScalable, TMsgEntityCreated, onCreation);
}

void TCompScalable::onCreation(const TMsgEntityCreated& msgC) {
  //get initial scale
  TCompTransform* cTransform = get<TCompTransform>();
  initial_scale = cTransform->getScale();
  //calculate speed with all the data
  speed = (final_scale - initial_scale) / time_to_scale;
}

void TCompScalable::renderDebug() {

}

void TCompScalable::update(float delta) {

  TCompTransform* cTransform = get<TCompTransform>();
  float actual_scale = cTransform->getScale();
  actual_scale = clamp(actual_scale + speed * delta, -final_scale, final_scale);

  cTransform->setScale(actual_scale);
}


