#include "mcv_platform.h"
#include "module_game_manager.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"
#include "components/controllers/character/comp_character_controller.h"

bool CModuleGameManager::start()
{
	
	isPaused = false; //estamos pausados por ganar, perder o menu pausa
	victoryMenu = false; //el menu de ganar
	menuDead = false; // el menu de muerte
	menuVisible = false; ;//el menu dentro dl juego
	
	//no mostrar puntero raton

	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	windowWidth = 250;
	windowHeight = 70;
	
	
		
	Input::CMouse mouse = EngineInput.mouse();
	mouse.setLockMouse(true);
	

	return true;
}

void CModuleGameManager::update(float delta)
{
	gameCondition();
	
}

void CModuleGameManager::stop()
{

}

void CModuleGameManager::gameCondition() {
	/*ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs;*/
	CHandle h_player = getEntityByName("Player");
	CEntity * e_player = h_player;
	if (e_player != nullptr) {
		TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
		if (c_controller->life <= 0) {

			
			/*
			window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
			float window_size_width = 300;
			float window_size_height = 55;
			ImGui::SetNextWindowPos(ImVec2(Render.width / 2 - window_size_width / 2, Render.height / 2 - window_size_height / 2));
			ImGui::SetNextWindowSize(ImVec2(window_size_width, window_size_height));
			ImGui::Begin("Game Over", nullptr, window_flags);
			ImGui::Text("GAME OVER, press key T for continue");
			ImGui::End();*/

			Input::CMouse mouse = EngineInput.mouse();
			mouse.setLockMouse(false);
			
			menuDead = true;
			

			TMsgGamePause msg;
			msg.isPause = true;
			msg.playerDead = true;
			CEntity* cam_player = getEntityByName("PlayerCamera");
			if (cam_player != nullptr) {
				cam_player->sendMsg(msg);
			}

			
		}
		//si estas vivo y no es pausa seear el puntero
		//if (c_controller->endGame) {

		if (c_controller->endGame) {
			/*window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
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
			ImGui::End();*/
			Input::CMouse mouse = EngineInput.mouse();
			mouse.setLockMouse(false);
			victoryMenu = true;
			TMsgGamePause msg;
			msg.isPause = true;
			msg.playerDead = true;
			CEntity* cam_player = getEntityByName("PlayerCamera");
			if(cam_player != nullptr){
				cam_player->sendMsg(msg);
			}
		}

	}
	
	if (!isPaused && EngineInput["pause"].justPressed() || (!menuVisible && CApplication::get().lostFocus())) {
		
	
		//juego en pausa y jugador vivo

		isPaused = true;
		CApplication::get().setLostFocus(false);
        GameController.pauseGame();
		//envio de mensaje de pausa
		
		// hacemos visualizar puntero en el menu
		Input::CMouse mouse = EngineInput.mouse();
		mouse.setLockMouse(false);

		menuVisible = true;
		Time.real_scale_factor = 0.0f;
	}
	else if (isPaused && EngineInput["pause"].justPressed()) {

		Input::CMouse mouse = EngineInput.mouse();
		mouse.setLockMouse(true);
		//jugador no muerto y juego en pausa
		exitPauseGame();//quitamos pausa
        GameController.resumeGame();
    }
}




void CModuleGameManager::renderInMenu()
{

}

void CModuleGameManager::renderDebug()
{

	
	VEC2 menu_position = VEC2(float(CApplication::get().width_app) * .5f - (windowWidth * .5f), CApplication::get().height_app * .5f - (windowHeight * .5f));

	
	if (menuVisible) {

		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
		ImGui::Begin("MENU", false, window_flags);
		ImGui::CaptureMouseFromApp(false);
		ImGui::SetWindowPos("MENU", ImVec2(menu_position.x, menu_position.y));

		ImGui::Selectable("Resume", menuPos == 0);
		if (ImGui::IsItemClicked() || (menuPos == 0 && EngineInput["menu_confirm"].justPressed()))
		{
			exitPauseGame();
		}

		ImGui::Selectable("Restart checkpoint", menuPos == 1);
		if (ImGui::IsItemClicked() || (menuPos == 1 && EngineInput["menu_confirm"].justPressed()))
		{
			exitPauseGame();
			CEntity* e_player = getEntityByName("Player");
			if (!e_player) {
				return;
			}
			
			TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
			if (c_controller->life > 0) {
				//quitar puntero de raton
				menuDead = false;
				c_controller->ChangeState("GROUNDED");
				GameController.loadCheckpoint();
				

			}
		}

	
		ImGui::Selectable("Exit game", menuPos == 2);
		if (ImGui::IsItemClicked() || (menuPos == 2 && EngineInput["btMenuConfirm"].justPressed()))
		{
			auto& app = CApplication::get();
			DestroyWindow(app.getHandle()); 
		}

		ImGui::End();
	}
	else if (victoryMenu) {

		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
		ImGui::Begin("VICTORY!", false, window_flags);
		ImGui::CaptureMouseFromApp(false);
		ImGui::SetWindowPos("VICTORY!", ImVec2(menu_position.x, menu_position.y));
		ImGui::Text("Congratulations guy!");
		ImGui::Selectable("Exit game", menuPos == 1);
		if (ImGui::IsItemClicked() || (menuPos == 1 && EngineInput["btMenuConfirm"].justPressed()))
		{
			auto& app = CApplication::get();
			DestroyWindow(app.getHandle());
		}
		ImGui::End();

	}
	else if (menuDead) {


		/*
		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
		ImGui::Begin("VICTORY!", false, window_flags);
		ImGui::CaptureMouseFromApp(false);
		ImGui::SetWindowPos("VICTORY!", ImVec2(menu_position.x, menu_position.y));
		ImGui::Text("Congratulations guy!");
		ImGui::Selectable("Exit game", menuPos == 1);
		if (ImGui::IsItemClicked() || (menuPos == 1 && EngineInput["btMenuConfirm"].justPressed()))
		{
			auto& app = CApplication::get();
			DestroyWindow(app.getHandle());
		}
		ImGui::End();
		
		*/
		
		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
		ImGui::Begin("YOU DIED", false, window_flags);
		ImGui::CaptureMouseFromApp(false);
		ImGui::SetWindowPos("YOU DIED", ImVec2(menu_position.x, menu_position.y));
		
		ImGui::Selectable("Restart checkpoint", menuPos == 1);
		if (ImGui::IsItemClicked() || (menuPos == 1 && EngineInput["btMenuConfirm"].justPressed()))
		{
			CEntity* e_player = getEntityByName("Player");
			if (!e_player) {
				return;
			}
			TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
			if ((c_controller->life <= 0)) {
				Input::CMouse mouse = EngineInput.mouse();
				mouse.setLockMouse(true);
				//quitar puntero de raton
				menuDead = false;
				c_controller->ChangeState("GROUNDED");
				GameController.loadCheckpoint();

				TMsgGamePause msg;
				msg.isPause = false;
				msg.playerDead = false;
				CEntity* cam_player = getEntityByName("PlayerCamera");
				if (cam_player != nullptr) {
					cam_player->sendMsg(msg);
				}


			}
		}


		ImGui::Selectable("Exit game", menuPos == 2);
		if (ImGui::IsItemClicked() || (menuPos == 2 && EngineInput["btMenuConfirm"].justPressed()))
		{
			auto& app = CApplication::get();
			DestroyWindow(app.getHandle());
		}

		ImGui::End();

	}
}


void CModuleGameManager::exitPauseGame() {
	// quitamos pausa
	isPaused = false;
	CApplication::get().setLostFocus(false);

	
	Time.real_scale_factor = 1.0f;

	// Quitar puntero de raton
	

	menuVisible = false;
	Input::CMouse mouse = EngineInput.mouse();
	mouse.setLockMouse(true);
}
