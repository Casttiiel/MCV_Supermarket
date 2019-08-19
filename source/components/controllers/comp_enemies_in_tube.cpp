#include "mcv_platform.h"
#include "comp_enemies_in_tube.h"
#include "components/controllers/character/comp_character_controller.h"
#include "modules/game/module_game_controller.h"
#include "engine.h"
#include "components/ai/bt/bt_cupcake.h"
DECL_OBJ_MANAGER("enemies_in_tube", TCompEnemiesInTube);

void TCompEnemiesInTube::debugInMenu() {
  
}


void TCompEnemiesInTube::registerMsgs() {
	DECL_MSG(TCompEnemiesInTube, TMSgWallDestroyed, onFinishEnemiesInTube);
}

void TCompEnemiesInTube::onFinishEnemiesInTube(const TMSgWallDestroyed & msg) {
	if (msg.isDetroyed) {
		activateTrap = false;
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
}


void TCompEnemiesInTube::load(const json& j, TEntityParseContext& ctx){
	enemiesPosition.push_back(VEC3(24,10,-6));//Esto cambiarlo mas adelanta por los nombres de los tubos
	enemiesPosition.push_back(VEC3(14,10,-6));
	enemiesPosition.push_back(VEC3(13,10,3));
	enemiesPosition.push_back(VEC3(24,10,2));
}


void TCompEnemiesInTube::update(float dt) {
	if(activateTrap){
		if (birthTime > 0) {
			birthTime -= dt;
		}
		else {
			CHandle handle = GameController.spawnPrefab("data/prefabs/enemies/bt_cupcake.json", enemiesPosition[positionBirth], QUAT(0, 0, 0, 1), 1);
			CEntity* entity = (CEntity *)handle;
			CBTCupcake* cupcake = entity->get<CBTCupcake>();
			cupcake->setHalfCone(360);
			cupcake->setLengthCone(300);
			positionBirth++;
			if (positionBirth >= enemiesPosition.size() ) {
				positionBirth = 0;
			}
			birthTime = birthTimeReset;
		}
	}
}

