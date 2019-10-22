#include "mcv_platform.h"
#include "module_game_intro.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"

bool CModuleGameIntro::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	CEngine::get().getUI().activateWidgetClass("INTRO_SCREEN")->childAppears(true, true, 0.0, 1.25);
	
	//Scripting.execActionDelayed("activateWidget(\"LOAD_SCREEN\")", 1.5);
	//Scripting.execActionDelayed("changeGameState(\"gs_loading\")", 1.5);
	return true;
}

void CModuleGameIntro::update(float delta)
{
	
}

void CModuleGameIntro::stop()
{
	
}

void CModuleGameIntro::renderInMenu()
{

}

void CModuleGameIntro::renderDebug()
{

}
