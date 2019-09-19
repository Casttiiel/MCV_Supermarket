#include "mcv_platform.h"
#include "module_game_loading.h"
#include "engine.h"
#include "input/input.h"
#include "modules/module_boot.h"
#include "components/common/comp_transform.h"
#include "ui/controllers/ui_menu_controller.h"
#include "ui/module_ui.h"
#include "ui/widgets/ui_button.h"

bool CModuleGameLoading::start()
{
	
  return true;
}

void CModuleGameLoading::update(float delta)
{
  PROFILE_FUNCTION("CModuleGameLoading::update");
  if (Engine.getBoot().isLoadAll ) {
	  CEngine::get().getModules().changeToGamestate("gs_gameplay");
  }
  else {
	  CEngine::get().getModules().changeToGamestate("gs_boot");
  }
  
}

void CModuleGameLoading::stop()
{
	CEngine::get().getUI().deactivateWidgetClass("LOAD_SCREEN");
	
}

void CModuleGameLoading::renderInMenu()
{

}

void CModuleGameLoading::renderDebug()
{

}
