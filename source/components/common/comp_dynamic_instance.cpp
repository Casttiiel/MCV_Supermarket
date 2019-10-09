#include "mcv_platform.h"
#include "comp_dynamic_instance.h"
#include "engine.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("dynamic_instance", TCompDynamicInstance);

void TCompDynamicInstance::debugInMenu() {
  ImGui::LabelText("IDX: ", "%d", my_unique_idx);
}

void TCompDynamicInstance::load(const json& j, TEntityParseContext& ctx) {
}

void TCompDynamicInstance::renderDebug() {
}

void TCompDynamicInstance::onPlayerAttack(const TMsgDamage& msg) {
    TCompTransform* c_trans = get<TCompTransform>();

    VEC3 hit_pos = msg.position;
    VEC3 dir = c_trans->getPosition() - hit_pos;
    dir.y *= 0.1f;
    dir.Normalize();
    dir *= msg.impactForce * 2.0f;
    physx::PxVec3 impulse = VEC3_TO_PXVEC3(dir);
    
    TCompCollider* c_coll = get<TCompCollider>();
    physx::PxRigidDynamic* rigid_dynamica = static_cast<physx::PxRigidDynamic*>(c_coll->actor);
    rigid_dynamica->addForce(impulse, PxForceMode::eIMPULSE, false);

}

void TCompDynamicInstance::registerMsgs() {
  DECL_MSG(TCompDynamicInstance, TMsgDamage, onPlayerAttack);
  DECL_MSG(TCompDynamicInstance, TMsgEntityCreated, onCreate);
  DECL_MSG(TCompDynamicInstance, TMsgGravity, onBattery);
}

void TCompDynamicInstance::onBattery(const TMsgGravity& msg) {
  TCompCollider* c_cc = get<TCompCollider>();
  TCompTransform* c_trans = get<TCompTransform>();
  physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_cc->actor);
  VEC3 dir = c_trans->getPosition() - msg.position;
  dir.Normalize();
  dir *= 10.0f;
  rigid_dynamic->addForce(VEC3_TO_PXVEC3(dir), PxForceMode::eIMPULSE);

}

void TCompDynamicInstance::onCreate(const TMsgEntityCreated&) {
  TCompCollider* c_collider = get<TCompCollider>();
  h_coll = CHandle(c_collider);
}

void TCompDynamicInstance::update(float delta) {
  TCompCollider* c_collider = (TCompCollider*) h_coll;

  if (c_collider) {
    rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_collider->actor);

    //si no esta en reposo...
    if (!rigid_dynamic->isSleeping()) {
      //cojes el modulo de gpu culling y actualizas los datos de este obj (transform y aabb)
      //assert(my_unique_idx > -1);
      if (my_unique_idx < 0)
        return;
      CHandle h(this);
      CEngine::get().getGPUCulling().updateObjData(my_unique_idx, h.getOwner());
      //pones is_dirty = true
      CEngine::get().getGPUCulling().setDirty();
    }
  }
}
