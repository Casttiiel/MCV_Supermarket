#include "mcv_platform.h"
#include "module_game_intro.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"
#include "ui/ui_widget.h"




bool CModuleGameIntro::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	CEngine::get().getUI().activateWidgetClass("INTRO_SCREEN")->childAppears(true, true, 0.0, 1.25);
	//UI::CWidget* widgetPadre = ui.getWidget("INTRO_SCREEN");
	//UI::CWidget* widgetHijo = widgetPadre->getChildren(0)->getChildren(0);
	//UI::CEffect* effect = widget->getChildren(0)->getChildren(0)->getEffect("effectAnimateComic");
	//effect->stopUiFx();

	UI::CWidget* widget = ui.getWidget("INTRO_SCREEN");
	
	Scripting.execActionDelayed("stopWidgetEffectSpecial()", 2);
	Scripting.execActionDelayed("changeSpeedWidgetEffectSpecial(-0.85, 0.5)", 3);
	Scripting.execActionDelayed("stopWidgetEffectSpecial()",5);


	Scripting.execActionDelayed("childAppears(\"LOAD_SCREEN\",true,true,0.0,1.0)", 6);
	Scripting.execActionDelayed("changeGameState(\"gs_loading\")", 8);
	return true;
}

void CModuleGameIntro::update(float delta)
{
	
}

void CModuleGameIntro::stop()
{
	CEngine::get().getUI().deactivateWidgetClass("INTRO_SCREEN");
	CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
}

void CModuleGameIntro::renderInMenu()
{

}

void CModuleGameIntro::renderDebug()
{

}
