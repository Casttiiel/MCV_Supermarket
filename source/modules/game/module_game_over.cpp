#include "mcv_platform.h"
#include "module_game_over.h"
#include "engine.h"
#include "input/input.h"
#include "modules/module_boot.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"

bool CModuleGameOver::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 1.0);
		CEngine::get().getUI().activateWidgetClass("DEAD_MENU_BACKGROUND")->childAppears(true, true, 0.0, 1.0);
		CEngine::get().getUI().activateWidgetClass("DEAD_MENU_BUTTONS");
		CEngine::get().getUI().deactivateWidgetClass("HUD_NORMAL_PLAYER");
	}
	else {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 1.0);
		CEngine::get().getUI().activateWidgetClass("DEAD_MENU_BACKGROUND_MINI")->childAppears(true, true, 0.0, 1.0);
		CEngine::get().getUI().activateWidgetClass("DEAD_MENU_BUTTONS_MINI");
		CEngine::get().getUI().deactivateWidgetClass("HUD_NORMAL_PLAYER_MINI");
	}

    return true;
}

void CModuleGameOver::update(float delta)
{
	
}

void CModuleGameOver::stop()
{
	
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
		CEngine::get().getUI().deactivateWidgetClass("DEAD_MENU_BACKGROUND");
		CEngine::get().getUI().deactivateWidgetClass("DEAD_MENU_BUTTONS");
		CEngine::get().getUI().activateWidgetClass("HUD_NORMAL_PLAYER");
	}
	else {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
		CEngine::get().getUI().deactivateWidgetClass("DEAD_MENU_BACKGROUND_MINI");
		CEngine::get().getUI().deactivateWidgetClass("DEAD_MENU_BUTTONS_MINI");
		CEngine::get().getUI().activateWidgetClass("HUD_NORMAL_PLAYER");
	}
}

void CModuleGameOver::renderInMenu()
{

}

void CModuleGameOver::renderDebug()
{

}
