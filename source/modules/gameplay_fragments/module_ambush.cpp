#include "mcv_platform.h"
#include "module_ambush.h"
#include "components/common/comp_transform.h"

bool CModuleAmbush::start()
{
  //SAVE HERE THE INITIAL INFO OF THE SCENE/SITUATION
	firstEvent();

  return true;
}


void CModuleAmbush::stop()
{
  //JUST IN CASE SOMETHING IS NEEDED HERE
}

void CModuleAmbush::enemyKilled(EntityType e) {
  if (e == EntityType::SUSHI) {
    sushisKilled++;
  }

  if (sushisKilled == 1) {
	  secondEvent();
  }
  else if (sushisKilled == 3) {
    //TODO
  }
}

void CModuleAmbush::renderInMenu()
{
    ImGui::Text("Ambush gameplay fragment:");
    ImGui::LabelText("thermostats activates: ","%d", termostatosActives);
}

void CModuleAmbush::firstEvent() {

	termostatosActives++;
	Scripting.execActionDelayed("defrost1()", 0.0);
	Scripting.execActionDelayed("setTransformObject_(\"Box009\",VEC3(-28.075,33.429,-71.601),70,0,-21.600)",5.0);
	Scripting.execActionDelayed("wakeUpSuishisSecondFloor()", 0.0);
  //CONGELAR ENEMIGOS Y PLAYER
  //CINEMATICA
  //ACTIVAR PLATAFORMAS(SUELO)
  //DESCONGELAR ENEMIGOS Y PLAYER

	//Scripting.runScript(AMBUSH_EVENT);
}

void CModuleAmbush::secondEvent() {
	Scripting.execActionDelayed("defrost2()", 0.0); 
	Scripting.execActionDelayed("on_ambush()", 0.0); 
	//descongelar suishis finales
  //CONGELAR ENEMIGOS Y PLAYER
  //CINEMATICA
  //ACTIVAR "BOMBARDEO"
  //DESCONGELAR ENEMIGOS Y PLAYER
}

int CModuleAmbush::getThermostatsActivates() {
	return termostatosActives;
}


