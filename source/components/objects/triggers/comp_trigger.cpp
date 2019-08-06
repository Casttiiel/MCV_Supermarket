#include "mcv_platform.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "engine.h"
#include "comp_trigger.h"
#include "components/ai/others/comp_blackboard.h"


DECL_OBJ_MANAGER("trigger", TCompTrigger);

void TCompTrigger::debugInMenu() {
}
/*este componente hara que dependiendo de lo que le pongas
  en el in_script (entrar en el trigger) o 
  en el out_script (salir en el trigger) por 
*/
void TCompTrigger::load(const json& j, TEntityParseContext& ctx) {

	//in_script = j.value("in_script", "");
    //out_script = j.value("out_script", "");
	//type = j.value("type", type);//entiendo que esto es el nombre del trigger
	nameTrigger = j.value("nameTrigger", nameTrigger);
	onEnter_ = j.value("onEnter", onEnter_);
	onExit_ = j.value("onExit", onExit_);
	delay = j.value("delay", delay);
}

//solo onEnter o en Exit si en el json existe
void TCompTrigger::registerMsgs()
{
	DECL_MSG(TCompTrigger, TMsgEntityTriggerEnter, onEnter);
	DECL_MSG(TCompTrigger, TMsgEntityTriggerExit, onExit);
}


void TCompTrigger::onExit(const TMsgEntityTriggerExit & msg) 
{
	//sales de trigger
	
	if (onExit_.length() > 0) {
		dbg("Se entra en trigger \n");
		CHandle entity = msg.h_entity;
		if (entity == GameController.getPlayerHandle()) { //es el player
			
			std::string params = onExit_ + "_player()";
			//de este palo ...Scripting.execEvent(GameController::Events::TRIGGER_ENTER, params);
			Scripting.execEvent(Events::TRIGGER_EXIT, params, delay);
		}
		else {
			CEntity * c_other = msg.h_entity;
			std::string other_name = c_other->getName();
			std::string params = onExit_ + "_enemy(" + other_name + ")";//por ejemplo
			Scripting.execEvent(Events::TRIGGER_EXIT, params, delay);
		}
	}

  //Scripting.runScript(DEACTIVATE_GAMEPLAY_FRAGMENT,"ambush"); //pass a parameter here "\"ambush\""
}


//me cuadra mas tener un execEvent con el evento ONTRIGGER por ejemplo y el param como el nombre del trigger en el module script de la siguiente forma: 
//bool CModuleScripting::execEvent(Events event, const std::string & params, float delay) 
void TCompTrigger::onEnter(const TMsgEntityTriggerEnter & msg)
{

	//PROFILE_SET_NFRAMES(3);
	//PROFILE_FRAME_BEGINS();
	//PROFILE_FUNCTION("onEnter");
	//entras en trigger
	if (onEnter_.length() > 0) {
		CHandle entity = msg.h_entity;
		if (entity == GameController.getPlayerHandle()) { //es el player
			dbg("Player entra en trigger \n");
			std::string params = onEnter_ + "_player()";
			//de este palo ...Scripting.execEvent(GEvents::TRIGGER_ENTER, params);
			Scripting.execEvent(Events::TRIGGER_ENTER, params, delay);
		}
		else {
			CEntity * c_other = msg.h_entity;
			std::string other_name = c_other->getName();
			std::string params = onEnter_ + "_enemy";//por ejemplo
			//dbg("------%s\n",params.c_str());
			//dbg("------%s\n", other_name.c_str());//(" + other_name + ")"
			Scripting.runScript(params, other_name , delay);
			//Scripting.execEvent(Events::TRIGGER_ENTER, params, other_name, delay);
		}
	}
	/*
	CHandle h_player = GameController.getPlayerHandle();
	if (msg.h_entity == h_player) {
		dbg("Entra en el trigger \n");//temporal hasta que el exportador distinga entre triggerss
		if (type==0) {
			Scripting.runScript(ACTIVATE_SCENE, in_script); //pass a parameter here "\"ambush\""
			
			CEntity* e_player = (CEntity*)h_player;
			TCompBlackboard* c_bb = e_player->get<TCompBlackboard>();
			if (c_bb->getNumTermostatesBroken() == 1) { //un termostato esta destruido
				Scripting.runScript(ACTIVE_SUSHIS, "true");
			}


		}
		else if (type==1) {
			Scripting.runScript(DELETE_GRIETAS, "grieta_001",10.f);
			Scripting.runScript(DELETE_GRIETAS, "grieta_002", 25.f);
			Scripting.runScript(DELETE_GRIETAS, "grieta_003", 40.f);

		}
		
	}
	*/
 
}

void TCompTrigger::update(float dt)
{
	//dbg("trigger update\n");
}
