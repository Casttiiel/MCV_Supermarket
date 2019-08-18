#include "mcv_platform.h"
#include "comp_bolt_sphere.h"
#include "components/common/comp_transform.h"

DECL_OBJ_MANAGER("bolt_sphere", TCompBoltSphere);

void TCompBoltSphere::debugInMenu() {

}

void TCompBoltSphere::load(const json& j, TEntityParseContext& ctx) {
  speed = j.value("speed", speed);
}


void TCompBoltSphere::renderDebug() {

}

void TCompBoltSphere::registerMsgs() {
  DECL_MSG(TCompBoltSphere, TMsgEntityCreated, onCreation);
}

void TCompBoltSphere::onCreation(const TMsgEntityCreated& msgC) {
  TCompTransform* c_trans = get<TCompTransform>();
  if (c_trans) {
    c_trans = get<TCompTransform>();
    VEC3 pos = c_trans->getPosition();
    target_position = pos + VEC3(0,1,0);
  }
}

void TCompBoltSphere::update(float delta) {

  TCompTransform* cTransform = get<TCompTransform>();

  VEC3 dir = target_position - cTransform->getPosition();
  dir.Normalize();
  cTransform->setPosition(cTransform->getPosition() + dir * delta * speed);

}


