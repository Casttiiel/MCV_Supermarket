#include "mcv_platform.h"
#include "module_game_ui.h"
#include "entity/entity.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_transform.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_bar.h"
#include "components/controllers/character/comp_character_controller.h"
#include "components/powers/comp_madness.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/widgets/ui_button.h"

bool CModuleGameUI::start()
{
	UI::CModuleUI& ui = Engine.getUI();
	ui.activateWidget("game_ui");


	UI::CMenuController* menu = new UI::CMenuController;
	/*
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_carro_")), std::bind(&CModuleGameUI::onOptionCarrito, this));
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_dash_")), std::bind(&CModuleGameUI::onOptionDash, this));
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_jump_")), std::bind(&CModuleGameUI::onOptionJump, this));
	menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_mop_")), std::bind(&CModuleGameUI::onOptionMop, this));
	*/
	

	ui.registerController(menu);



  return true;
}

void CModuleGameUI::update(float delta)
{
  PROFILE_FUNCTION("CModuleGameUI::update");
  
  if (EngineInput["pause"].justPressed()) {
    CEngine::get().getModules().changeToGamestate("gs_paused");//change gamestate
    //pause game
    Time.real_scale_factor = 0.0f;
	
  }
  if (EngineInput["jump_"].isPressed() && EngineInput["jump_"].timePressed < 0.5f) {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_jump_"));
	  boton->setCurrentState("selected");
  }
  else {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_jump_"));
	  boton->setCurrentState("enabled");
  }

  if (EngineInput["dash_"].isPressed() && EngineInput["dash_"].timePressed < 0.5f) {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_dash_"));
	  boton->setCurrentState("selected");
  }
  else {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_dash_"));
	  boton->setCurrentState("enabled");
  }

  if (EngineInput["interact_"].isPressed() && EngineInput["interact_"].timePressed < 0.5f) {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_carro_"));
	  boton->setCurrentState("selected");
  }
  else {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_carro_"));
	  boton->setCurrentState("enabled");
  }
  if (EngineInput["attack_"].isPressed() && EngineInput["attack_"].timePressed < 0.5f) {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_mop_"));
	  boton->setCurrentState("selected");
  }
  else {
	  UI::CButton* boton = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("bt_mop_"));
	  boton->setCurrentState("enabled");
  }

  CEntity* e_player = getEntityByName("Player");
  if (e_player != nullptr) {
		//Mana
		TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
		TCompMadnessController* madness_controller = e_player->get<TCompMadnessController>();
		float madness = (madness_controller->getRemainingMadness() + 20) / (c_controller->getMaxMadness() + 20) ;//ofset de las barras de vida
		UI::CBar* bar = dynamic_cast<UI::CBar*>(Engine.getUI().getWidgetByAlias("mana_bar_r"));
		bar->setRatio(madness);
   }
  
}

void CModuleGameUI::onOptionCarrito() {
	dbg("Opcion carrito \n");
}

void CModuleGameUI::onOptionDash() {
	dbg("Opcion dash \n");
}

void CModuleGameUI::onOptionJump() {
	dbg("Opcion jump \n");
}

void CModuleGameUI::onOptionMop() {
	dbg("Opcion mop \n");
}

void CModuleGameUI::stop()
{

}

void CModuleGameUI::renderInMenu()
{

}

void CModuleGameUI::renderDebug()
{

}