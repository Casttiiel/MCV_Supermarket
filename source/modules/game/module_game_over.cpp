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
	ui.activateWidgetClass("DEAD_MENU_BACKGROUND");
	ui.activateWidgetClass("DEAD_MENU_BUTTONS");
    return true;
}

void CModuleGameOver::update(float delta)
{
 
}

void CModuleGameOver::stop()
{
	UI::CModuleUI& ui = Engine.getUI();
	ui.deactivateWidgetClass("DEAD_MENU_BACKGROUND");
}

void CModuleGameOver::renderInMenu()
{

}

void CModuleGameOver::renderDebug()
{

}
