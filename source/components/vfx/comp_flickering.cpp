#include "mcv_platform.h"
#include "comp_flickering.h"
#include "components/common/comp_buffers.h"
#include "components/common/comp_light_point.h"
#include "random"

std::mt19937 bt_flick(1992);
std::uniform_int_distribution<int> bt_range2(0, 100);
std::uniform_int_distribution<int> bt_range3(0, 1);

DECL_OBJ_MANAGER("flickering", TCompFlickering);

void TCompFlickering::debugInMenu() {
  ImGui::DragFloat("Frequency", &frequency, 0.0f, 5.0f);
  ImGui::DragFloat("Amplitude", &amplitude, 0.0f, 5.0f);
  ImGui::DragFloat("Base", &base, 0.0f, 5.0f);
  ImGui::DragFloat("Phase ", &phase, 0.0f, 5.0f);
}

void TCompFlickering::load(const json& j, TEntityParseContext& ctx) {
  target = j.value("target", target);
  frequency = j.value("frequency", frequency);
  amplitude = j.value("amplitude", amplitude);
  base = j.value("base", base);
  phase = bt_range2(bt_flick); // start point inside on wave cycle
}

void TCompFlickering::renderDebug() {
}

void TCompFlickering::update(float delta) {
  float randomizer = bt_range3(bt_flick);
  float x = ((float)Time.current) * frequency * randomizer;
  float y;

  x = x - floor(x); // normalized value (0..1)

  y = sin(((x + phase/100.0f) * 2 * PI));

  float randomizer2 = bt_range3(bt_flick);
  float res = (y * amplitude * randomizer) + base;

  if (strcmp(target.c_str(), "point") == 0) {
    TCompLightPoint* c_lp = get<TCompLightPoint>();
    c_lp->setIntensity(res);
  } else if (strcmp(target.c_str(), "dir")) {

  }

}


