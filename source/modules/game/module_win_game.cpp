#include "mcv_platform.h"
#include "module_win_game.h"
#include "engine.h"
#include "input/input.h"
#include "modules/module_boot.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"
#include "module_game_controller.h"

bool CModuleWinGame::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 1.0);
	}
	else {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 1.0);
	}

    return true;
}

void CModuleWinGame::update(float delta)
{
	timeOffModuleGameWin--;
	if (timeOffModuleGameWin <= 0.f) {
		//GameController.changeGameState("gs_main_menu");
		auto& app = CApplication::get();
		DestroyWindow(app.getHandle());
	}

	
}

void CModuleWinGame::stop()
{
	
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
	}
	else {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
	}
}

void CModuleWinGame::renderInMenu()
{

}

void CModuleWinGame::renderDebug()
{

}