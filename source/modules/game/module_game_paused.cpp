#include "mcv_platform.h"
#include "module_game_paused.h"
#include "entity/entity.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"

bool CModuleGamePaused::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	ui.activateWidget("pausa_menu");

	UI::CMenuController* menu = new UI::CMenuController;

	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_continue_")), std::bind(&CModuleGamePaused::onOptionContinue, this));
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_restart_")), std::bind(&CModuleGamePaused::onOptionRestart, this));
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_exit_")), std::bind(&CModuleGamePaused::onOptionExit, this));

	menu->setCurrentOption(0);

	ui.registerController(menu);



  return true;
}

void CModuleGamePaused::update(float delta)
{
  PROFILE_FUNCTION("CModuleGamePaused::update");
  if (EngineInput["pause"].justPressed()) {
    CEngine::get().getModules().changeToGamestate("gs_gameplay");//change gamestate
    //unpause game
    Time.real_scale_factor = 1.0f;


  }
  
}

void CModuleGamePaused::stop()
{
	Engine.getUI().deactivateWidget("pausa_menu");
}

void CModuleGamePaused::renderInMenu()
{

}

void CModuleGamePaused::renderDebug()
{

}

void CModuleGamePaused::onOptionContinue() {
	
	Time.real_scale_factor = 1.0f;
	CEngine::get().getModules().changeToGamestate("gs_gameplay");//change gamestate
	UI::CModuleUI& ui = Engine.getUI();
	ui.unregisterController();
}

void CModuleGamePaused::onOptionRestart() {
	
	CEngine::get().getModules().changeToGamestate("gs_gameplay");//change gamestate
	Time.real_scale_factor = 1.0f;
	UI::CModuleUI& ui = Engine.getUI();
	ui.unregisterController();
	GameController.loadCheckpoint();

}

void CModuleGamePaused::onOptionExit() {
	auto& app = CApplication::get();
	DestroyWindow(app.getHandle());
	
}