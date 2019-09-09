#pragma once

#include "modules/module.h"

namespace UI
{
  class CWidget;
  class CController;

  class CModuleUI : public IModule
  {
  public:
    CModuleUI(const std::string& name);

	struct WidgetClass {
		std::string name;//nombre del widget en el json es el name
		std::string type; //Tipo de si es vida, stamina, boton, creditos, fondo negro... (lo que se nos antoje)
		CWidget *widget;
		CController *_controller;
		bool enabled = false;
	};

	std::map<std::string, WidgetClass> _widgetStructureMap;
    bool start() override;
    void update(float dt) override;
    void render();

    void renderInMenu() override;
	void unregisterController();
	void unregisterController(CController* controller);
    void registerWidget(CWidget* widget);
    void activateWidget(const std::string& name);
    void deactivateWidget(const std::string& name);
	//Nuevos metodos pra la causa
	void registerWidgetClass(std::string wdgt_type, std::string wdgt_path, CController *wdgt_controller = nullptr);
	void initWidgetClass();
	CWidget* activateWidgetClass(const std::string& name);
	void deactivateWidgetClass(const std::string& name);

	CController* getWidgetController(std::string type);
    void registerAlias(CWidget* widget);
    void registerController(CController* controller);

    CWidget* getWidgetByName(const std::string& name);
    CWidget* getWidgetByAlias(const std::string& alias);

  private:
    std::map<std::string_view, CWidget*> _registeredWidgets;
    std::map<std::string_view, CWidget*> _registeredAlias;
    std::vector<CWidget*> _activeWidgets;
    std::vector<CController*> _activeControllers;
  };
}
