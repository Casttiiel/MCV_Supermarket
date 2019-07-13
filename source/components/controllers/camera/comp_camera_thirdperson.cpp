#include "mcv_platform.h"
#include "comp_camera_thirdperson.h"
#include "components/common/comp_transform.h"
#include "components/controllers/character/comp_character_controller.h"
#include "utils/utils.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"

using namespace physx;
DECL_OBJ_MANAGER("camera_thirdperson", TCompCameraThirdPerson);

void TCompCameraThirdPerson::debugInMenu() {
  ImGui::DragFloat3("Camera offset", &offset.x, 0.1f, 0.0f, 6.f);
  ImGui::DragFloat("Sensitivity X", &sensitivity_x, 0.01f, 0.01f, 3.f);
  ImGui::DragFloat("Sensitivity Y", &sensitivity_y, 0.01f, 0.01f, 3.f);
  ImGui::DragFloat("Distance running", &distance_, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Distance aiming", &distance_2, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Speed", &enclosing_speed, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Decreasing factor", &decreasing_factor, 0.05f, 0.0f, 1.0f);
  ImGui::DragFloat("Rotation speed", &rotation_speed, 0.1f, 0.0f, 15.0f);
}

void TCompCameraThirdPerson::load(const json& j, TEntityParseContext& ctx) {
  sensitivity_x = j.value("sensitivity_x", sensitivity_x);
  sensitivity_y = j.value("sensitivity_y", sensitivity_y);
  distance_ = j.value("distance_", distance_);
  enclosing_speed = j.value("enclosing_speed", enclosing_speed);
  rotation_speed = j.value("rotation_speed", rotation_speed);



  if (j.count("key")) {
    std::string k = j["key"];
    _key_toggle_enabled = k[0];
  }
}

void TCompCameraThirdPerson::update(float scaled_dt)
{
 PROFILE_FUNCTION("ThrirdPersonCamera");
  TCompTransform* c_transform = get<TCompTransform>();

  if (!c_transform) {
    return;
  }

  float dt = scaled_dt * _timeScale;
  h_player = getEntityByName("Player");
  CEntity* e_player = (CEntity *)h_player;
  TCompTransform* player_position = e_player->get<TCompTransform>();

  VEC3 front = c_transform->getFront();
  float yaw, pitch;
  vectorToYawPitch(front, &yaw, &pitch);
  auto& mouse = EngineInput.mouse();
  VEC2 mOff = mouse.getDelta();

  pitch_rotation_speed *= decreasing_factor;
  yaw_rotation_speed *= decreasing_factor;

  //INPUT FROM MOUSE
  if (mOff.y != 0.0f) { pitch_rotation_speed = mOff.y * dt * sensitivity_y * rotation_speed; }
  if (mOff.x != 0.0f) { yaw_rotation_speed = -mOff.x * dt * sensitivity_x * rotation_speed; }

  if (EngineInput.gamepad()._connected) { //IF CONNECTED, INPUT FROM GAMEPAD
    if (EngineInput["rleft_"].value != 0.0f) { yaw_rotation_speed = -EngineInput["rleft_"].value * dt * sensitivity_x * rotation_speed; }
    if (EngineInput["up_"].value != 0.0f) { pitch_rotation_speed = -EngineInput["up_"].value * dt * sensitivity_y * rotation_speed; }
  }

  pitch += pitch_rotation_speed;
  yaw += yaw_rotation_speed;

  /*if (pitch <= angle_min_y)
    pitch = angle_min_y;
  else*/ if (pitch > angle_max_y)
    pitch = angle_max_y;

  c_transform->setAngles(yaw, pitch);

  TCompCharacterController* character_controller = e_player->get<TCompCharacterController>();
  if (character_controller->aiming && distance_to_player > distance_2) {
    distance_to_player -= dt * enclosing_speed;
  }
  else if (character_controller->aiming && distance_to_player < distance_2) {
    distance_to_player = distance_2;
  }
  else if (!character_controller->aiming && distance_to_player < distance_) {
    distance_to_player += dt * enclosing_speed;
  }
  else if (!character_controller->aiming && distance_to_player > distance_) {
    distance_to_player = distance_;
  }

  VEC3 vector = player_position->getPosition() + offset - front * distance_to_player;

  c_transform->setPosition(vector);

}

void TCompCameraThirdPerson::setTimeScale(float timeScale) {
	assert(timeScale >= 0.0);
	_timeScale = timeScale;
}
float TCompCameraThirdPerson::getTimeScale() {
	return _timeScale;
}