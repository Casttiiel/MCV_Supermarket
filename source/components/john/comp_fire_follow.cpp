#include "mcv_platform.h"

#include "comp_fire_follow.h"
#include "components/common/physics/comp_collider.h"
#include "components/controllers/character/comp_character_controller.h"
#include "components/powers/comp_fire.h"
#include "components/common/comp_render.h"

DECL_OBJ_MANAGER("fire_follow", TCompFireFollow);

void TCompFireFollow::debugInMenu() {

} 

void TCompFireFollow::load(const json& j, TEntityParseContext& ctx) {
}

void TCompFireFollow::update(float delta) {
  CEntity* en = getEntityByName("Player");
  if (!en)
      return;
  TCompTransform* c_trans_pl = en->get<TCompTransform>();

  TCompCollider* comp_collider = en->get<TCompCollider>();
  if (!comp_collider || !comp_collider->controller)
    return;

  float attackHeight = (float)comp_collider->controller->getHeight();
  float playerRadius = (float)comp_collider->controller->getRadius();

  TCompTransform* c_trans = get<TCompTransform>();
  c_trans->setPosition(c_trans_pl->getPosition() + VEC3(0.f, attackHeight, 0.f) + (c_trans_pl->getFront() * (playerRadius)));

  //get yaw from player
  float p_yaw, p_pitch;
  c_trans_pl->getAngles(&p_yaw, &p_pitch);

  float yaw, pitch;
  c_trans->getAngles(&yaw, &pitch);

  pitch = p_pitch;
  //get pitch from camera if it is aiming

  TCompCharacterController* p_contr = en->get<TCompCharacterController>();
  if (p_contr->aiming) {
    CEntity* cam = getEntityByName("PlayerCamera");
    TCompTransform* cam_trans = cam->get<TCompTransform>();
    float c_yaw, c_pitch;
    cam_trans->getAngles(&c_yaw, &c_pitch);
    pitch = c_pitch;
  }

  c_trans->setAngles(p_yaw, pitch);

  TCompFireController* c_fire = en->get<TCompFireController>();
  TCompRender* c_render = get<TCompRender>();
  if (c_fire->isEnabled()) {
    c_render->is_visible = true;
  }
  else {
    c_render->is_visible = false;
  }
  c_render->updateRenderManager();
}




