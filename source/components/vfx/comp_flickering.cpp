#include "mcv_platform.h"
#include "comp_flickering.h"
#include "components/common/comp_buffers.h"


DECL_OBJ_MANAGER("flickering", TCompFlickering);

void TCompFlickering::debugInMenu() {
  ImGui::LabelText("Target: ","%s",target.c_str());
  //ImGui::DragFloat("Speed", &speed, 0.0f, 5.0f);
}

void TCompFlickering::load(const json& j, TEntityParseContext& ctx) {
  target = j.value("target", target);
}

void TCompFlickering::renderDebug() {
}

void TCompFlickering::update(float delta) {
  /*if (!_enabled) {
    ratio = 0.0f;
  }
  else {
    if (ratio < 1.0f) {
      ratio = clamp(ratio + speed * delta * 2.0f, 0.0f, 2.0f);
    }
    else {
      ratio = clamp(ratio + speed * delta * 0.35f, 0.0f, 2.0f);
    }
  }
  
  TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    constants.x = Interpolator::cubicInOut(0.0f, 1.0f, ratio);
    constants.y = Interpolator::linear(0.f, 1.f, clamp(ratio - 1.0f, 0.0f, 1.0f));

    auto buf = c_buff->getCteByName("TCtesSmokeDust");
    buf->updateGPU(&constants);
  }

  if (ratio >= 2.0f)
    CHandle(this).getOwner().destroy();*/
}


