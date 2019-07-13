#include "mcv_platform.h"
#include "module_main_menu.h"
#include "engine.h"
#include "input/input.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_progress.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/widgets/ui_button.h"

CModuleMainMenu::CModuleMainMenu(const std::string& name)
  : IModule(name)
{}

bool CModuleMainMenu::start()
{
  UI::CModuleUI& ui = Engine.getUI();

  ui.activateWidget("main_menu");

  UI::CMenuController* menu = new UI::CMenuController;

  menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_start")), std::bind(&CModuleMainMenu::onOptionStart, this));
  menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_continue")), std::bind(&CModuleMainMenu::onOptionContinue, this));
  menu->registerOption(dynamic_cast<UI::CButton*>(ui.getWidgetByAlias("bt_exit")), std::bind(&CModuleMainMenu::onOptionExit, this));

  menu->setCurrentOption(0);

  ui.registerController(menu);

  return true;
}

void CModuleMainMenu::onOptionStart()
{
  //Engine.getModules().changeToGamestate("gameplay");
  printf("START");
  CEngine::get().getModules().changeToGamestate("gs_gameplay");
  Engine.getSceneManager().loadScene("tutorial_scene"); //carga de la primera escena
  UI::CModuleUI& ui = Engine.getUI();
  ui.unregisterController();
}

void CModuleMainMenu::onOptionContinue()
{
  //Engine.getModules().changeToGamestate("gameplay");
  printf("CONTINUE");
}

void CModuleMainMenu::onOptionExit()
{
  printf("EXIT");
  auto& app = CApplication::get();
  DestroyWindow(app.getHandle());
  
}

void CModuleMainMenu::stop()
{
  Engine.getUI().deactivateWidget("main_menu");
}

void CModuleMainMenu::update(float dt)
{
  static float ratio = 1.f;
  if (EngineInput[VK_RIGHT])
  {
    ratio = clamp(ratio + 1.f * dt, 0.f, 1.f);
    UI::CProgress* progressBar = dynamic_cast<UI::CProgress*>(Engine.getUI().getWidgetByAlias("life_bar"));
    progressBar->setRatio(ratio);
  }

  if (EngineInput[VK_LEFT])
  {
    ratio = clamp(ratio - 1.f * dt, 0.f, 1.f);
    UI::CProgress* progressBar = dynamic_cast<UI::CProgress*>(Engine.getUI().getWidgetByAlias("life_bar"));
    progressBar->setRatio(ratio);
  }


  if(EngineInput[VK_F1])
  {
    CEngine::get().getModules().changeToGamestate("gs_gameplay");
  }
}
