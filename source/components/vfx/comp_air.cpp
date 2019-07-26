#include "mcv_platform.h"
#include "comp_air.h"
#include "components/common/comp_buffers.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("air", TCompAir);

void TCompAir::debugInMenu() {

}

void TCompAir::load(const json& j, TEntityParseContext& ctx) {
}

void TCompAir::renderDebug() {
}

void TCompAir::update(float delta) {
  /*TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    constants.x = Interpolator::cubicInOut(0.0f, 1.0f, ratio);
    constants.y = Interpolator::cubicOut(0.f, 1.f, clamp(ratio - 1.0f, 0.0f, 1.0f));

    auto buf = c_buff->getCteByName("TCtesAir");
    buf->updateGPU(&constants);
  }*/
  TCompTransform* c_trans = get<TCompTransform>();
  c_trans->setPosition(c_trans->getPosition() + c_trans->getFront() * delta * speed);
}


