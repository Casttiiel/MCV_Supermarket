#include "mcv_platform.h"
#include "engine.h"
#include "comp_fire_trap.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/vfx/comp_air.h"
#include "components/ai/others/self_destroy.h"

DECL_OBJ_MANAGER("comp_fire_trap", TCompFireTrap);

void TCompFireTrap::debugInMenu() {
 /* ImGui::DragFloat("Wind Frequency: ", &_windDelay, 0.01f, 0.f, 1.f);
  ImGui::DragFloat("Scale Variation: ", &_scaleVar, 0.1f, 0.1f, 2.f);
  ImGui::DragFloat("Speed Variation: ", &_speedVar, 0.1f, 0.1f, 3.f);
  ImGui::DragFloat("Min Destroy Time: ", &_startDestroy, 0.1f, 0.1f, 5.f);
  ImGui::DragFloat("Destroy Variation: ", &_destroyVar, 0.1f, 0.1f, 2.f);
  ImGui::DragFloat("Wind Visibility: ", &windLength, 0.1f, 0.1f, 3.f);
  ImGui::DragFloat("Wind Decay: ", &windDist, 0.1f, 0.1f, 2.f);*/
}

void TCompFireTrap::load(const json& j, TEntityParseContext& ctx) {
}

void TCompFireTrap::registerMsgs() {
  DECL_MSG(TCompFireTrap, TMsgEntityCreated, onCreate);
}

void TCompFireTrap::onCreate(const TMsgEntityCreated& msg) {
  TCompTransform* c_trans = get<TCompTransform>();
  
  //emision position and direction
  TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    auto buf = c_buff->getCteByName("TCtesParticles");
    CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
    data->emitter_center = c_trans->getPosition();
    data->emitter_dir = c_trans->getFront();
    data->emitter_num_particles_per_spawn = 4;
    data->updateGPU();
  }
}

void TCompFireTrap::enable() {
  TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    auto buf = c_buff->getCteByName("TCtesParticles");
    CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
    data->emitter_num_particles_per_spawn = 4;
    data->updateGPU();
  }
}

void TCompFireTrap::disable() {
  TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    auto buf = c_buff->getCteByName("TCtesParticles");
    CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
    data->emitter_num_particles_per_spawn = 0;
    data->updateGPU();
  }
}

void TCompFireTrap::update(float dt) {

  if (_isEnabled) {
    if (_fireTimer <= 0.f) {
      TCompTransform* c_trans = get<TCompTransform>();
      TEntityParseContext ctx;
      ctx.root_transform.setPosition(c_trans->getPosition());
      float yaw, pitch, roll;
      c_trans->getAngles(&yaw, &pitch, &roll);

      ctx.root_transform.setAngles(yaw, pitch, roll);
      parseScene("data/prefabs/bullets/fireball.json", ctx);

      _fireTimer = _fireDelay;
    }
    else {
      _fireTimer -= dt;
    }

    _disablerTimer -= dt;
    if (_disablerTimer <= 0.f) {
      _isEnabled = false;
      disable();
      _disablerTimer = _disablerDelay;
    }
  }
  else {
    _enablerTimer -= dt;
    if (_enablerTimer <= 0.f) {
      _isEnabled = true;
      enable();
      _enablerTimer = _enablerDelay;
    }
  }
}
