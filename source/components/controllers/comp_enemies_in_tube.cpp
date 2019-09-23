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


void TCompEnemiesInTube::load(const json& j, TEntityParseContext& ctx) {
	enemiesPosition.push_back(VEC3(18.214, 9.143, -7.335));
	enemiesPosition.push_back(VEC3(27.714, 9.143, -7.335));
	enemiesPosition.push_back(VEC3(27.714, 9.143, 5.790));
	enemiesPosition.push_back(VEC3(18.064, 9.143, 5.890));



}


void TCompEnemiesInTube::update(float dt) {

	if (activateTrap) {
		if (birthTime > 0) {
			birthTime -= dt;

			CEntity* entity_emis = (CEntity*)h1;
			if (entity_emis != nullptr) {
				TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
				if (c_buff) {
					auto buf = c_buff->getCteByName("TCtesParticles");
					CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
					data->emitter_num_particles_per_spawn = 5;
					data->updateGPU();
				}
			}
		}
		else {
			CHandle handle = GameController.spawnPrefab("data/prefabs/enemies/bt_cupcake.json", enemiesPosition[positionBirth], QUAT(0, 0, 0, 1), 1);
			if (positionBirth == 0) {
				h1 = GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
			}
			if (positionBirth == 1) {
				h1 = GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
			}
			if (positionBirth == 2) {
				h1 = GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
			}
			if (positionBirth == 3) {
				h1 = GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);
			}
			CEntity* entity_emis = (CEntity*)h1;
			TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
			if (c_buff) {
				auto buf = c_buff->getCteByName("TCtesParticles");
				CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
				data->emitter_num_particles_per_spawn = 50;
				data->updateGPU();
			}

			if (!firstTime) {
				/*GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);*/
				firstTime = true;
			}

			CEntity* entity = (CEntity *)handle;
			CBTCupcake* cupcake = entity->get<CBTCupcake>();
			cupcake->setHalfCone(360);
			cupcake->setLengthCone(300);
			positionBirth++;
			if (positionBirth >= enemiesPosition.size()) {
				positionBirth = 0;
			}
			birthTime = birthTimeReset;
		}
	}
}



//CODIGO ANTIGUO

/*
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
	enemiesPosition.push_back(VEC3(18.214,9.143,-7.335));
	enemiesPosition.push_back(VEC3(27.714, 9.143, -7.335));
	enemiesPosition.push_back(VEC3(27.714, 9.143, 5.790));
	enemiesPosition.push_back(VEC3(18.064, 9.143, 5.890));
	


}


void TCompEnemiesInTube::update(float dt) {
	if(activateTrap){
		if (birthTime > 0) {
			birthTime -= dt;
		}
		else {
			CHandle handle = GameController.spawnPrefab("data/prefabs/enemies/bt_cupcake.json", enemiesPosition[positionBirth], QUAT(0, 0, 0, 1), 1);
			if(!firstTime){
				GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);
				firstTime = true;
			}

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

*/