#include "mcv_platform.h"
#include "comp_enemies_in_butcher.h"
#include "components/controllers/character/comp_character_controller.h"
#include "modules/game/module_game_controller.h"
#include "engine.h"

DECL_OBJ_MANAGER("enemies_in_butcher", TCompEnemiesInButcher);

void TCompEnemiesInButcher::debugInMenu() {
  
}


void TCompEnemiesInButcher::registerMsgs() {
	DECL_MSG(TCompEnemiesInButcher, TMSgEnemyDead, onSushiTrap);
}


void TCompEnemiesInButcher::load(const json& j, TEntityParseContext& ctx){
	
}


void TCompEnemiesInButcher::update(float dt) {
	if (count >= 3) {
		Scripting.execActionDelayed("activeElevatorInitCarniceria_player()", 0.0);
		//desactivar llamas de la plataforma cuando este listo
		GameController.destroyCHandleByName("enemies_in_butcher");
		
	}
}

void TCompEnemiesInButcher::onSushiTrap(const TMSgEnemyDead & msg) {
	CHandle handle = msg.h_entity;
	CEntity* entity = (CEntity*) handle;
	TCompName* name = entity->get<TCompName>();
	std::string nameEntity = name->getName();
	if(nameEntity.compare("Sushi000") == 0) {//los de nombre diferente  son los suishi de las plantas de arriba
		count++;
	}


}

