#include "mcv_platform.h"
#include "module_win_game.h"
#include "engine.h"
#include "render/render.h"
#include "input/input.h"
#include "modules/module_boot.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"
#include "module_game_controller.h"
#include "render/module_render.h"

bool CModuleWinGame::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 2.0);
		Scripting.execActionDelayed("activateWidget(\"CREDITS\")",0);
		Scripting.execActionDelayed("changeSpeedWidgetEffect(\"CREDITS\",\"effectAnimateCredit\",0,0.012)", 1);
		Scripting.execActionDelayed("stopWidgetEffect(\"CREDITS\",\"effectAnimateCredit\")", 72);
		//Scripting.execActionDelayed("exitGame()",82);
		Scripting.execActionDelayed("changeGameState(\"gs_main_menu\")", 74.0);

	}
	else {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 2.0);
	}

	
	//Scripting.execActionDelayed("changeGameState(\"gs_main_menu\")", 1.0);
	return true;
}

void CModuleWinGame::update(float delta)
{
	/*timeOffModuleGameWin--;
	if (timeOffModuleGameWin <= 0.f) {
		//GameController.changeGameState("gs_main_menu");
		auto& app = CApplication::get();
		DestroyWindow(app.getHandle());
	}*/

	/*if (!creditos) {
		CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true, true, 0.0, 2.0);
		Scripting.execActionDelayed("activateWidget(\"CREDITS\")", 0);
		Scripting.execActionDelayed("changeSpeedWidgetEffect(\"CREDITS\",\"effectAnimateCredit\",0,0.012)", 1);
		Scripting.execActionDelayed("stopWidgetEffect(\"CREDITS\",\"effectAnimateCredit\")", 72);
		//Scripting.execActionDelayed("exitGame()", 82);
		Scripting.execActionDelayed("changeGameState(\"gs_main_menu\")", 82.0);
	}
	creditos = true;*/
}

void CModuleWinGame::stop()
{

	UI::CModuleUI& ui = Engine.getUI();
	if (ui.sizeUI == 1) {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
		//UI::CWidget* widget = ui.getWidget("CREDITS");
		UI::CWidget* widget = ui.getWidget("CREDITS");
		//UI::CWidget* widgetHijo = widget->getChildren(0)->getChildren(0);
		UI::CEffect* effect = widget->getEffect("effectAnimateCredit");
		effect->setMaxUV(VEC2::One);
		effect->setMinUV(VEC2::Zero);
		effect->changeSpeedUV(0.0,0.15);
		CEngine::get().getUI().deactivateWidgetClass("CREDITS");


	}
	else {
		CEngine::get().getUI().deactivateWidgetClass("BLACK_SCREEN");
	}


	UI::CButton* b = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("card_"));
	b->setCurrentState("option_teleport");
	//cambiar imagen de cursor un instante de timepo
	UI::CButton* b_cursor = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("cursor_"));
	b_cursor->setCurrentState("option_teleport");

	GameController.updateAmbientLight(0.300);
	EngineNavmesh.destroyNavmesh();
	EngineEntities.stop();
	//EnginePhysics.stop();
	//GameController.deactivateGameplayFragment("ambush"),//deactivateGameplayFragment("ambush");
	Engine.getGPUCulling().stop();
	GameController.updateSoundtrackID(1);

	Resources.deleteResources();

	Engine.getBoot().isLoadAll = false;

	auto& mod_render = CEngine::get().getRender();

	mod_render.start();
	

	




}

void CModuleWinGame::renderInMenu()
{

}

void CModuleWinGame::renderDebug()
{

}
