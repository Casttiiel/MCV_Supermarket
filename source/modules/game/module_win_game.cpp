#include "mcv_platform.h"
#include "module_win_game.h"
#include "engine.h"
#include "input/input.h"
#include "modules/module_boot.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"

bool CModuleWinGame::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		
	}
	else {
		
	}

    return true;
}

void CModuleWinGame::update(float delta)
{
	
}

void CModuleWinGame::stop()
{
	
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		
	}
	else {
		
	}
}

void CModuleWinGame::renderInMenu()
{

}

void CModuleWinGame::renderDebug()
{

}
