#include "mcv_platform.h"
#include "comp_bolt_sphere.h"
#include "components/common/comp_transform.h"

DECL_OBJ_MANAGER("bolt_sphere", TCompBoltSphere);

void TCompBoltSphere::debugInMenu() {

}

void TCompBoltSphere::load(const json& j, TEntityParseContext& ctx) {

}


void TCompBoltSphere::renderDebug() {

}

void TCompBoltSphere::update(float delta) {

  TCompTransform* cTransform = get<TCompTransform>();

  CEntity* e_player = getEntityByName("Player");
  TCompTransform* c_trans_player = e_player->get<TCompTransform>();
  cTransform->rotateTowards(c_trans_player->getPosition());
  cTransform->inclineTo(c_trans_player->getPosition());
  float dist = VEC3::Distance(c_trans_player->getPosition(), VEC3::Zero);
  cTransform->setScale(dist);

}


