#include "mcv_platform.h"
#include "comp_skel_lookat_direction.h"
#include "components/common/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "cal3d/cal3d.h"
#include "cal3d2engine.h"
#include "entity/entity_parser.h"
#include "game_core_skeleton.h"
#include "components/controllers/character/comp_character_controller.h"

DECL_OBJ_MANAGER("skel_lookat_direction", TCompSkelLookAtDirection);

void TCompSkelLookAtDirection::load(const json& j, TEntityParseContext& ctx) {
  /*if( j.count("target"))
    target = loadVEC3( j["target"] );*/
  amount = j.value("amount", amount);
  target_transition_factor = j.value("target_transition_factor", target_transition_factor);
}

void TCompSkelLookAtDirection::setDirection(VEC3 direction) {
    dir = direction;
}

void TCompSkelLookAtDirection::setDefaultDirection(VEC3 defaultDirection) {
    default_dir = defaultDirection;
}

void TCompSkelLookAtDirection::update(float dt) {
  return;
  TCompSkeleton* c_skel = h_skeleton;
  if (target == VEC3().Zero) {
    TCompTransform* c_trans = get<TCompTransform>();
    target = c_trans->getPosition() + (c_trans->getFront() * positionOffset);
  }
  TCompCharacterController* c_c = get < TCompCharacterController>();
  if (!c_c->aiming) {
    return;
  }
  // If we have a target direction, use it to assign a target position
    // We should ensure the target position is not moving abruptly
    // to avoid large changes in the skeleton reorientation
    // make the target smoothly change between positions
  //c_trans->getPosition() + (c_trans->getFront() * chargeSpeed * chargeDuration);
  TCompTransform* c_trans = get<TCompTransform>();
  if (c_trans == nullptr) {
	  return;
  }
    VEC3 new_target = c_trans->getPosition() + (dir * positionOffset);
    target = target * target_transition_factor + new_target * (1.0f - target_transition_factor);

  if (c_skel == nullptr) {
    // Search the parent entity by name
    CEntity* e_entity = CHandle(this).getOwner();
    if (!e_entity)
      return;
    // The entity I'm tracking should have an skeleton component.
    h_skeleton = e_entity->get<TCompSkeleton>();
    assert(h_skeleton.isValid());
    c_skel = h_skeleton;
  }

  // The cal3d skeleton instance
  CalSkeleton* skel = c_skel->model->getSkeleton();

  // The set of bones to correct
  auto core = (CGameCoreSkeleton*)c_skel->model->getCoreModel();
  for (auto& it : core->lookat_direction_corrections)
    it.apply(skel, target, amount);
}

void TCompSkelLookAtDirection::renderDebug() {
  drawMesh(Resources.get("unit_wired_cube.mesh")->as<CMesh>(), MAT44::CreateTranslation(target), VEC4(1, 0, 0, 1));
}

void TCompSkelLookAtDirection::debugInMenu() {
  ImGui::InputFloat3("Target", &target.x);
  ImGui::LabelText( "Target Name", "%s", target_entity_name.c_str() );
  ImGui::DragFloat("Amount", &amount, 0.01f, 0.f, 1.0f);
  ImGui::DragFloat("Transition Factor", &target_transition_factor, 0.01f, 0.f, 1.0f);
}
