#include "mcv_platform.h"
#include "comp_parabolic_launch.h"
#include "components/common/comp_transform.h"
#include "components/controllers/character/comp_character_controller.h"
#include "components/controllers/camera/comp_aim_controller.h"
#include "components/common/comp_render.h"
#include "input/input.h"
#include "components/powers/comp_madness.h"
DECL_OBJ_MANAGER("parabolic_launch", TCompParabolicLaunch);

void TCompParabolicLaunch::debugInMenu() {
  ImGui::DragFloat("Velocity", &v0, 0.1f, 1.f, 100.f);
  /*ImGui::DragFloat("Sensitivity", &_sensitivity, 0.001f, 0.001f, 0.1f);
  ImGui::DragFloat("Inertia", &_ispeed_reduction_factor, 0.001f, 0.7f, 1.f);
  ImGui::LabelText("Curr Speed", "%f", _ispeed);*/
}

void TCompParabolicLaunch::load(const json& j, TEntityParseContext& ctx){
	v0 = j.value("velocity", v0);
}

void TCompParabolicLaunch::renderDebug()
{

	/*if (paintCurve) {
		if (curve_dynamic)
		{
			CTransform tr;
			curve_dynamic->renderDebug(tr);
		}
	}*/
	//Importante.Cambiar de sitio cuando tengamos UI de Albert
	if (EngineInput["fire_attack_"].isPressed()) { 
		paintCurve = false;
		CEntity* ui = getEntityByName("Mirilla");
		TCompRender* c_render = ui->get<TCompRender>();
		c_render->is_visible = false;
		c_render->updateRenderManager();
		
	}
	else {
		if (paintCurve) {
			if (curve_dynamic)
			{
				CTransform tr;
				curve_dynamic->renderDebug(tr);
			}
			CEntity* ui = getEntityByName("Mirilla");
			TCompRender* c_render = ui->get<TCompRender>();
			c_render->is_visible = false;
			c_render->updateRenderManager();
		}
		else {
			if (!h_player.isValid()) {
          h_player = getEntityByName("Player");
				return;
			}//
			TCompCharacterController* c_player = ((CEntity*)h_player)->get<TCompCharacterController>();
			if (c_player->aiming && c_player->power_selected == PowerType::TELEPORT) {
				CEntity* ui = getEntityByName("Mirilla");
				TCompRender* c_render = ui->get<TCompRender>();
				c_render->is_visible = true;
				c_render->updateRenderManager();


			}

		}
	}
}

void TCompParabolicLaunch::update(float dt) {
  if (!h_player.isValid()) {
    e_player = getEntityByName("Player");
    return;
  }

	  TCompCharacterController* c_player = ((CEntity*)h_player)->get<TCompCharacterController>();
	  if (c_player->aiming && c_player->power_selected == PowerType::BATTERY && c_player->unLockableBattery) {
		CEntity* e_camera = getEntityByName("PlayerCamera");
		TCompTransform* transformPlayer = ((CEntity*)e_player)->get<TCompTransform>();
		TCompTransform* transformC_aim = e_camera->get<TCompTransform>();
		paintCurve = true;
		// 1.80  mide pj
		float yaw, pitch;
		transformC_aim->getAngles(&yaw, &pitch);
		float yawPj, pitchPj;
		transformPlayer->getAngles(&yawPj, &pitchPj);
		loadValuesParabola(heightPj, pitch, yawPj);
	
	  }
	  else {
		paintCurve = false;
	  }
}

void TCompParabolicLaunch::calculateParabolicPoints() {
	curve_dynamic = new CCurve();
	for (int i = 1; i < 100; i++) {
		float y = h + v0y * (i / v0x) - 0.5 *  (9.8) * pow((i / v0x), 2);
		const VEC3 operacion = VEC3(0, y, i);
		if (!e_player.isValid()) { return; }
		TCompTransform* cTargetTransform = ((CEntity*)e_player)->get<TCompTransform>();
		const MAT44 mRotation = MAT44::CreateFromYawPitchRoll(yaw, 0.f, 0.f);
		const MAT44 mTranslation = MAT44::CreateTranslation(cTargetTransform->getPosition() + cTargetTransform->getFront() * 0.5f);
		MAT44 resultMatrix = (mRotation * mTranslation * MAT44::Identity);
		VEC3 result;
		Vector3::Transform(operacion, resultMatrix, result);
		curve_dynamic->addKnot(result);
	}
}

void TCompParabolicLaunch::loadValuesParabola(float new_h, float new_pitch,float new_yaw) {
	h = new_h;
	pitch = new_pitch;
	yaw = new_yaw;
	v0x = v0 * cosf(-1 * pitch);
	v0y = v0 * sinf(-1 * pitch);
	calculateParabolicPoints();
}
