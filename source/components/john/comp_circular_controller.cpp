#include "mcv_platform.h"
#include "comp_circular_controller.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_buffers.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"

DECL_OBJ_MANAGER("circular_controller", TCompCircularController);

// Example on how to customize the 'updateAll' for the components of type TCompCircularController
// In this case we send also the index
void CObjectManager< TCompCircularController > ::updateAll( float dt ) {
  for (uint32_t i = 0; i < num_objs_used; ++i)
    objs[i].update(dt, i);
}

void TCompCircularController::debugInMenu() {
  ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 10.f);
  ImGui::DragFloat("Radius", &radius, 0.1f, 0.f, 10.f);
  ImGui::LabelText("Yaw", "%f", current_yaw);
  ImGui::DragFloat3("Center", &center.x, 0.01f, -10.0f, 10.0f);
  if (ImGui::DragFloat4("Morph Weights", &morph_weights.a, 0.01f, 0.0, 1.0f)) {
    TCompBuffers* c_buff = get<TCompBuffers>();
    if (c_buff) {
      auto buf = c_buff->getCteByName("TCtesMorph");
      buf->updateGPU(&morph_weights);
    }
  }
} 

void TCompCircularController::load(const json& j, TEntityParseContext& ctx) {
  speed = j.value("speed", speed);
  radius = j.value("radius", radius);
  current_yaw = j.value("yaw", current_yaw);
  center = loadVEC3(j, "center");
}

void TCompCircularController::registerMsgs() {
  DECL_MSG(TCompCircularController, TMsgEntityCreated, onEntityCreated);
}

void TCompCircularController::onEntityCreated(const TMsgEntityCreated& msg) {
  dbg("Hi from onEntityCreated\n");
}

void TCompCircularController::update(float delta, uint32_t idx) {
  current_yaw += delta * speed;
  VEC3 new_pos = yawToVector(current_yaw) * radius + center;

  TCompTransform* c_trans = get<TCompTransform>();
  if (!c_trans)
    return;
  c_trans->setPosition(new_pos);
  c_trans->setAngles(current_yaw, 0.0f, 0.f);

  if (EngineInput['T'].justPressed()) {
    TEntityParseContext ctx;
    ctx.root_transform = *c_trans;
    parseScene("data/prefabs/bullet.json", ctx);

    TMsgAssignBulletOwner msg;
    msg.h_owner = CHandle(this).getOwner();
    msg.source = c_trans->getPosition();
    msg.front = c_trans->getFront();

    ctx.entities_loaded[0].sendMsg(msg);
  }

  
  // destroy my entity
  // CHandle(this).getOwner().destroy();
  
  // destroy myself
  // CHandle(this).destroy();

}


