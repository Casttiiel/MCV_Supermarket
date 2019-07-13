#include "mcv_platform.h"
#include "comp_follow_light.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("follow_light", TCompFollowLight);

DXGI_FORMAT readFormat(const json& j, const std::string& label);

// -------------------------------------------------
void TCompFollowLight::debugInMenu() {
  ImGui::DragFloat3("Offset ", &offset.x, 0.1f, -80.f, 80.f);
}

// -------------------------------------------------
void TCompFollowLight::renderDebug() {

}

// -------------------------------------------------
void TCompFollowLight::load(const json& j, TEntityParseContext& ctx) {
  offset = loadVEC3(j, "offset");
}

void TCompFollowLight::update(float dt) {
  if (!e_player.isValid())
  {
    e_player = getEntityByName("Player");
    return;
  }

  TCompTransform* c_player = ((CEntity *)e_player)->get<TCompTransform>();
  TCompTransform* c = get<TCompTransform>();
  if (!c || !c_player)
    return;
  //c->lookAt(c_player->getPosition() + offset, c_player->getPosition());
}
