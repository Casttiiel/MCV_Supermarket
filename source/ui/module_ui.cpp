#include "mcv_platform.h"
#include "engine.h"
#include "ui/module_ui.h"
#include "ui/ui_parser.h"
#include "ui/ui_widget.h"
#include "ui/ui_controller.h"
#include "ui/controllers/ui_menu_controller.h"
#include "windows/app.h"


namespace UI
{
  CModuleUI::CModuleUI(const std::string& name)
    : IModule(name)
  {
  }

  bool CModuleUI::start()
  {
    CParser parser;
    //parser.loadFile("data/ui/ui.json");
	initWidgetClass();
    return true;
  }

  void CModuleUI::update(float dt)
  {
	  
    for (auto controller : _activeControllers)
    {
      controller->update(dt);
    }

    for (auto widget : _activeWidgets)
    {
      widget->update(dt);
    }


	if (widgetsToLerp.size() > 0) { 
		
		std::vector<WidgetToLerp>::iterator it = widgetsToLerp.begin();
		while (it != widgetsToLerp.end()) {
			if ((*it).currentTime >= (*it).initialTime) {
				if ((*it).isFirstFrame) {
					(*it).maxElement = *(*it).element;
					(*it).isFirstFrame = false;
				}
				float diff = (*it).value - (*it).maxElement;
				float percentage = clamp(((((*it).currentTime - (*it).initialTime)) / (*it).lerpTime), 0.0f, 1.0f);
				*(*it).element = (*it).maxElement + (diff * percentage);
			}
			(*it).currentTime += dt;

			if (((*it).currentTime - (*it).initialTime) >= (*it).lerpTime) {
				*(*it).element <= (*it).value;
				it = widgetsToLerp.erase(it);
			}
			else {
				it++;
			}
		}
		
	}



  }
  //nuevos metodos
  void CModuleUI::registerWidgetClass(std::string wdgt_type, std::string wdgt_path, CController *wdgt_controller) {//Controller *wdgt_controller = nullptr

	  WidgetClass wdgt_class;
	  CParser parser;
	  wdgt_class.name = parser.loadFileByName(wdgt_path);
	  wdgt_class.type = wdgt_type;
	  wdgt_class.widget = getWidgetByName(wdgt_class.name);
	  wdgt_class._controller = wdgt_controller;
	  _widgetStructureMap[wdgt_type] = wdgt_class;
  }


  void CModuleUI::initWidgetClass() {
	  /*MENU PRINCIPAL*/

	  auto mpNewGame = []() {//nuevo juego
		  //CEngine::get().getModules().changeToGamestate("gs_gameplay");
		  
		  UI::CModuleUI& ui = Engine.getUI();
		  CEngine::get().getUI().activateWidgetClass("LOAD_SCREEN")->childAppears(true, true, 0.0, 1);
		  //CEngine::get().getUI().activateWidgetClass("LOAD_SPRITE")->childAppears(true, true, 0.0, 1);
		  //ejecutar dede LUA el gs_loading
		  Scripting.execActionDelayed("changeGameState(\"gs_loading\")", 1.5);
		  //CEngine::get().getModules().changeToGamestate("gs_loading");
	  };

	  auto mpCredits = []() {
		  CEngine::get().getUI().activateWidgetClass("BLACK_SCREEN")->childAppears(true,true,0.0,1.0);
		  CEngine::get().getUI().activateWidgetClass("LOAD_SPRITE")->childAppears(true, true, 0.0, 1.0);
	  };


	  auto mpExitGame = []() {
		  auto& app = CApplication::get();
		  DestroyWindow(app.getHandle());
	  };
	  UI::CModuleUI& ui = Engine.getUI();
	  registerWidgetClass("MAIN_MENU_BACKGROUND", "data/ui/widgets/main_menu_background.json", nullptr);
	  CMenuController* mmb = new CMenuController();
	  registerWidgetClass("MAIN_MENU_BUTTONS", "data/ui/widgets/main_menu_buttons.json", mmb);
	  mmb = (CMenuController*)getWidgetController("MAIN_MENU_BUTTONS");
	  mmb->registerOption("bt_start", mpNewGame);
	  //mmb->registerOption("bt_continue", mpCredits);
	  //mmb->registerOption("bt_exit", mpExitGame);
	  mmb->setCurrentOption(0);


	   /*MENU PAUSA*/
	  auto mpauseContinue = []() {
		  Time.real_scale_factor = 1.0f;
		  CEngine::get().getModules().changeToGamestate("gs_gameplay");//change gamestate
		  /*UI::CModuleUI& ui = Engine.getUI();
		  ui.unregisterController();*/
		  UI::CModuleUI& ui = Engine.getUI();
		  if (ui.sizeUI == 1) {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS");
		  }
		  else {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND_MINI");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS_MINI");
		  }
		  GameController.resumeGame();
	  };

	  auto mpauseRestart = []() {
		  CEngine::get().getModules().changeToGamestate("gs_gameplay");//change gamestate
		  Time.real_scale_factor = 1.0f;
		  /*UI::CModuleUI& ui = Engine.getUI();
		  ui.unregisterController();*/
		  UI::CModuleUI& ui = Engine.getUI();
		  if (ui.sizeUI == 1) {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS");
		  }
		  else {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND_MINI");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS_MINI");
		  }
		  GameController.loadCheckpoint();
	  };


	  auto  mpauseExitGame = []() {
		  UI::CModuleUI& ui = Engine.getUI();
		  if (ui.sizeUI == 1) {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS");
		  }
		  else {
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BACKGROUND_MINI");
			  CEngine::get().getUI().deactivateWidgetClass("PAUSE_MENU_BUTTONS_MINI");
		  }
		  auto& app = CApplication::get();
		  DestroyWindow(app.getHandle());
	  };


	 
	  


	  //GAMEPLAY SIN CARRITO TAMAÑO NORMAL Y PAUSA
	  json cfg = loadJson("data/config.json");
	  const json& cfg_render = cfg["render"];
	  int render_width = cfg_render.value("width", 1280);
	 
	  if(render_width >= 1920){
		  
		  ui.sizeUI = 1;

		  registerWidgetClass("PAUSE_MENU_BACKGROUND", "data/ui/widgets/pausa_menu_background.json", nullptr);
		  CMenuController* mpauseb = new CMenuController();
		  registerWidgetClass("PAUSE_MENU_BUTTONS", "data/ui/widgets/pausa_menu_buttons.json", mpauseb);
		  mpauseb = (CMenuController*)getWidgetController("PAUSE_MENU_BUTTONS");
		  mpauseb->registerOption("bt_continue_pause", mpauseContinue);
		  mpauseb->registerOption("bt_restart_pause", mpauseRestart);
		  mpauseb->registerOption("bt_exit_pause", mpauseExitGame);
		  mpauseb->setCurrentOption(0);


		  registerWidgetClass("HUD_NORMAL_PLAYER", "data/ui/widgets/game_ui.json", nullptr);
	  }
	  else {
		  ui.sizeUI = 0;

		  registerWidgetClass("PAUSE_MENU_BACKGROUND_MINI", "data/ui/widgets/pausa_menu_background_mini.json", nullptr);
		  CMenuController* mpauseb = new CMenuController();
		  registerWidgetClass("PAUSE_MENU_BUTTONS_MINI", "data/ui/widgets/pausa_menu_buttons_mini.json", mpauseb);
		  mpauseb = (CMenuController*)getWidgetController("PAUSE_MENU_BUTTONS_MINI");
		  mpauseb->registerOption("bt_continue_pause", mpauseContinue);
		  mpauseb->registerOption("bt_restart_pause", mpauseRestart);
		  mpauseb->registerOption("bt_exit_pause", mpauseExitGame);
		  mpauseb->setCurrentOption(0);
		  
		  registerWidgetClass("HUD_NORMAL_PLAYER_MINI", "data/ui/widgets/game_ui_mini.json", nullptr);



	  }
	 
	  //PANTALLA EN NEGRO
	  registerWidgetClass("BLACK_SCREEN", "data/ui//widgets/black_background.json", nullptr);
	  //PANTALLA DE CARGA
	  registerWidgetClass("LOAD_SCREEN", "data/ui/widgets/load_background.json", nullptr);
	  //SPRITE
	  registerWidgetClass("LOAD_SPRITE", "data/ui/widgets/loading_sprite.json", nullptr);


  }


  CWidget* CModuleUI::activateWidgetClass(const std::string& name) {
	  WidgetClass wdgtClass = _widgetStructureMap[name];
	  if (wdgtClass.enabled) return nullptr;
	  CWidget* widget = getWidgetByName(wdgtClass.name);
	  if (widget)
	  {
		  widget->onActivate();
		  wdgtClass.enabled = true;
		  _widgetStructureMap[name] = wdgtClass;
		  _activeWidgets.push_back(widget);

		  if (wdgtClass._controller != nullptr) {
			  registerController(wdgtClass._controller);
		  }
		  return widget;
	  }
	  else {
		  return nullptr;
	  }
  }
  void CModuleUI::deactivateWidgetClass(const std::string& name) {
	  WidgetClass wdgtClass = _widgetStructureMap[name];
	  CWidget* widgt = getWidgetByName(wdgtClass.name);
	  for (auto it = _activeWidgets.begin(); it != _activeWidgets.end();) {
		  if (*it == widgt) {

			  (*it)->onDeactivate();
			  _activeWidgets.erase(it);
			  break;
		  }
		  it++;
	  }

	  wdgtClass.enabled = false;
	  _widgetStructureMap[name] = wdgtClass;
	  if (wdgtClass._controller != nullptr) {
		  unregisterController(wdgtClass._controller);
	  }
  }


  //hasta aqui, los nuevos metodos
  void CModuleUI::render()
  {
    for (auto widget : _activeWidgets)
    {
      widget->doRender();
    }
  }

  void CModuleUI::renderInMenu()
  {
  }

  void CModuleUI::registerWidget(CWidget* widget)
  {
    assert(_registeredWidgets.find(widget->getName()) == _registeredWidgets.end());

    _registeredWidgets[widget->getName()] = widget;
  }

  void CModuleUI::registerAlias(CWidget* widget)
  {
    assert(_registeredAlias.find(widget->getAlias()) == _registeredAlias.end());

    _registeredAlias[widget->getAlias()] = widget;
  }
 
  void CModuleUI::activateWidget(const std::string& name)
  {
    CWidget* widget = getWidgetByName(name);
    if (widget)
    {
      _activeWidgets.push_back(widget);

      widget->start();
    }
  }

  void CModuleUI::deactivateWidget(const std::string& name)
  {
    auto it = std::find_if(_activeWidgets.begin(), _activeWidgets.end(), [&name](const CWidget* widget)
      {
        return widget->getName() == name;
      });
    if (it != _activeWidgets.end())
    {
      (*it)->stop();

      _activeWidgets.erase(it);
    }
  }

  void CModuleUI::registerController(CController* controller)
  {
    _activeControllers.push_back(controller);
  }


  //nuevo metodo tmb
  void CModuleUI::unregisterController(CController* controller)
  {
	  auto it = std::find(_activeControllers.begin(), _activeControllers.end(), controller);
	  if (it != _activeControllers.end())
	  {
		  _activeControllers.erase(it);
	  }
  }
  //hasta aqui


  void CModuleUI::unregisterController()
  {
	  _activeControllers.clear();
  }

  CWidget* CModuleUI::getWidgetByName(const std::string& name)
  {
    auto it = _registeredWidgets.find(name);
    return it != _registeredWidgets.end() ? it->second : nullptr;
  }


  CWidget* CModuleUI::getWidgetByAlias(const std::string& name)
  {
    auto it = _registeredAlias.find(name);
    return it != _registeredAlias.end() ? it->second : nullptr;
  }

  CController* CModuleUI::getWidgetController(std::string type) {
	  WidgetClass wdgt_class = _widgetStructureMap[type];
	  CController* controller = wdgt_class._controller;
	  return controller;
  }


  void CModuleUI::lerp(float *init_value, float value_to_lerp, float initial_time, float lerp_time) {
	  WidgetToLerp windgetToLerp;
	  windgetToLerp.element = init_value;
	  windgetToLerp.maxElement = *init_value;
	  windgetToLerp.value = value_to_lerp;
	  windgetToLerp.initialTime = initial_time;
	  windgetToLerp.currentTime = 0.0f;
	  windgetToLerp.lerpTime = lerp_time;
	  widgetsToLerp.push_back(windgetToLerp);
  }


}
