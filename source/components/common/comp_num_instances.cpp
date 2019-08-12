#include "mcv_platform.h"
#include "comp_num_instances.h"

DECL_OBJ_MANAGER("num_instances", TCompNumInstances);

void TCompNumInstances::debugInMenu() {
  ImGui::DragInt("Num Instances", (int*) &num_instances, 0.1f, 0, 2048 );
}

void TCompNumInstances::load(const json& j, TEntityParseContext& ctx) {
  num_instances = j.get<uint32_t>();
}

