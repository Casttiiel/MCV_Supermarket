#include "mcv_platform.h"
#include "engine.h"
#include "components/common/comp_transform.h"
#include "components/objects/comp_termoestato.h"


DECL_OBJ_MANAGER("comp_termoestato", TCompTermoestato);

void TCompTermoestato::debugInMenu() {



}

void TCompTermoestato::load(const json& j, TEntityParseContext& ctx) {

}

void TCompTermoestato::registerMsgs() {
	DECL_MSG(TCompTermoestato, TMsgGravity, onBattery);
}



void TCompTermoestato::onBattery(const TMsgGravity & msg) {
	_isEnabled = false;
    EngineAudio.playEvent("event:/Character/Powers/Battery/Glitch");
	//llamar a funcion de fragment
	int n = GameController.getNumThermoStatesActives();
	if(n == 1) {
		GameController.activateSecondEvent();
	}
	else{
		GameController.activateGameplayFragment("ambush");
	}
   //Animate or start particle system, do something
    /*
	if (!working) { //cambiar esto a u posible nuevo componenete termometro
		
		CHandle h_player = GameController.getPlayerHandle();
		CEntity* e_player = (CEntity *)h_player;
		TCompBlackboard* c_bb = e_player->get<TCompBlackboard>();
		c_bb->incrementTermostateBroken();
		

	}
	*/
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
}
	

