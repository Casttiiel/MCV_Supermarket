#include "mcv_platform.h"
#include "comp_fire_randomizer.h"
#include "components/common/comp_buffers.h"
#include "components/common/comp_light_point.h"
#include "random"

std::mt19937 bt_flick(1992);
std::uniform_int_distribution<int> bt_range2(0, 100);
std::uniform_int_distribution<int> bt_range3(0, 1);

DECL_OBJ_MANAGER("fire_randomizer", TCompFireRandomizer);

void TCompFireRandomizer::debugInMenu() {
  
}

void TCompFireRandomizer::load(const json& j, TEntityParseContext& ctx) {
  phase = bt_range2(bt_flick); // start point inside on wave cycle
}

void TCompFireRandomizer::renderDebug() {
}

void TCompFireRandomizer::update(float delta) {
  
}


