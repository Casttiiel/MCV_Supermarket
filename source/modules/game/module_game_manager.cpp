#include "mcv_platform.h"
#include "module_game_manager.h"
#include "entity/entity.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"
#include "components/controllers/character/comp_character_controller.h"

bool CModuleGameManager::start()
{
	



  return true;
}

void CModuleGameManager::update(float delta)
{
	updateGameCondition();
	
}

void CModuleGameManager::stop()
{

}

void CModuleGameManager::updateGameCondition() {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs;
	CHandle h_player = getEntityByName("Player");
	CEntity * e_player = h_player;
	if (e_player != nullptr) {
		TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
		if (c_controller->life <= 0) {
			window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
			float window_size_width = 300;
			float window_size_height = 55;
			ImGui::SetNextWindowPos(ImVec2(Render.width / 2 - window_size_width / 2, Render.height / 2 - window_size_height / 2));
			ImGui::SetNextWindowSize(ImVec2(window_size_width, window_size_height));
			ImGui::Begin("Game Over", nullptr, window_flags);
			ImGui::Text("GAME OVER, press key T for continue");
			ImGui::End();
		}
		if (c_controller->endGame) {
			window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
			float window_size_width = 500;
			float window_size_height = 55;
			ImGui::SetNextWindowPos(ImVec2(Render.width / 2 - window_size_width / 2, Render.height / 2 - window_size_height / 2));
			ImGui::SetNextWindowSize(ImVec2(window_size_width, window_size_height));
			ImGui::Begin("Game Over", nullptr, window_flags);
			ImGui::Text("Congratulations , the game has finished.\nPress ESC to exit the game.");
			auto& app = CApplication::get();

			if (EngineInput["exitGame_"].justPressed()) {
				DestroyWindow(app.getHandle());
			}
			ImGui::End();
		}

	}
}




void CModuleGameManager::renderInMenu()
{

}

void CModuleGameManager::renderDebug()
{

}
