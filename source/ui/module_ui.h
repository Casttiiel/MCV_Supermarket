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

    bool start() override;
    void update(float dt) override;
    void render();

    void renderInMenu() override;
	void unregisterController();
    void registerWidget(CWidget* widget);
    void activateWidget(const std::string& name);
    void deactivateWidget(const std::string& name);

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
