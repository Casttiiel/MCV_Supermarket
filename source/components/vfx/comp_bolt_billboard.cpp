#include "mcv_platform.h"
#include "comp_bolt_billboard.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_buffers.h"

DECL_OBJ_MANAGER("bolt_billboard", TCompBoltBillboard);

void TCompBoltBillboard::debugInMenu() {

}

void TCompBoltBillboard::load(const json& j, TEntityParseContext& ctx) {

}


void TCompBoltBillboard::renderDebug() {

}

void TCompBoltBillboard::update(float delta) {

  TCompTransform* cTransform = get<TCompTransform>();

  CEntity* e_player = getEntityByName("Player");
  TCompTransform* c_trans_player = e_player->get<TCompTransform>();
  cTransform->rotateTowards(c_trans_player->getPosition());
  cTransform->inclineTo(c_trans_player->getPosition());
  float dist = VEC3::Distance(c_trans_player->getPosition(), VEC3::Zero);
  cTransform->setScale(dist);

  if (prev_scale != dist) {
    constants.x = dist;
    TCompBuffers* c_buff = get<TCompBuffers>();
    if (c_buff) {
      auto buf = c_buff->getCteByName("TCtesBolt");
      buf->updateGPU(&constants);
    }
  }

  prev_scale = dist;
}


