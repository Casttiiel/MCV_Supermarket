#include "mcv_platform.h"
#include "ui/module_ui.h"
#include "ui/ui_parser.h"
#include "ui/ui_widget.h"
#include "ui/ui_controller.h"

namespace UI
{
  CModuleUI::CModuleUI(const std::string& name)
    : IModule(name)
  {
  }

  bool CModuleUI::start()
  {
    CParser parser;
    parser.loadFile("data/ui/ui.json");

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
  }

  void CModuleUI::registerWidgetClass(std::string wdgt_type, std::string wdgt_path, CController *wdgt_controller) {//Controller *wdgt_controller = nullptr

	  WidgetClass wdgt_class;
	  CParser parser;
	 // wdgt_class.path = parser.parseFile(wdgt_path);
	  wdgt_class.name = wdgt_type;
	 // wdgt_class.widget = getWidget(wdgt_struct._widgetName);
	  wdgt_class._controller = wdgt_controller;
	  _widgetStructureMap[wdgt_type] = wdgt_class;
  }



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
}
