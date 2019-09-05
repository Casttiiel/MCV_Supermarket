#include "mcv_platform.h"
#include "engine.h"
#include "comp_enemy_spawner_special_trap.h"
#include "components/common/comp_transform.h"
#include "components/ai/others/comp_blackboard.h"
#include "components/animation/comp_prop_animation.h"
#include "components/ai/bt/bt_cupcake.h"

DECL_OBJ_MANAGER("comp_enemy_spawner_special_trap", TCompEnemySpawnerSpecialTrap);

void TCompEnemySpawnerSpecialTrap::debugInMenu() {



}

void TCompEnemySpawnerSpecialTrap::load(const json& j, TEntityParseContext& ctx) {
	_prefab = j.value("_prefab", _prefab);
	_spawnMaxNumber = j.value("_spawnMaxNumber", _spawnMaxNumber);
	working = j.value("working", working);

}

void TCompEnemySpawnerSpecialTrap::registerMsgs() {

	DECL_MSG(TCompEnemySpawnerSpecialTrap, TMsgGravity, onBattery);
	DECL_MSG(TCompEnemySpawnerSpecialTrap, TMsgSpawnerCheckout, onCheckout);
}



void TCompEnemySpawnerSpecialTrap::onBattery(const TMsgGravity & msg) {
	_isEnabled = false;
  EngineAudio.playEvent("event:/Character/Powers/Battery/Glitch");
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

void TCompEnemySpawnerSpecialTrap::onCheckout(const TMsgSpawnerCheckout & msg) {
	for (auto it = _currentEnemies.begin(); it != _currentEnemies.end(); ++it) {
		if (*it == msg.enemyHandle) {
			_currentEnemies.erase(it);
			_spawnTimer = _spawnDelay;
			return;
		}
	}
}

void TCompEnemySpawnerSpecialTrap::update(float dt) {
	if (working) {
		if (_spawnTimer <= 0 && _currentEnemies.size() < _spawnMaxNumber) {
			dbg("Spawning %s\n", _prefab);
			TCompPropAnimator* animator = get<TCompPropAnimator>();
			animator->playAnimation(TCompPropAnimator::OVEN_OPEN, 1.f);
			TCompTransform* c_trans = get<TCompTransform>();
			VEC3 spawnPoint = c_trans->getPosition() + (c_trans->getFront() * _spawnOffset);
			CHandle enemy = GameController.spawnPrefab(_prefab, spawnPoint);
				
			//rotamos el cupcake hacia la posicion del jugador al spawnear para que te vea siempre
			CEntity* player = GameController.getPlayerHandle();
			CEntity* e_enemy = (CEntity*)enemy;
			TCompTransform* c_trans_enemy = e_enemy->get<TCompTransform>();
			if(player != nullptr) {
				TCompTransform* c_trans_player = player->get<TCompTransform>();
				c_trans_enemy->rotateTowards(c_trans_player->getPosition());
			}
				
			_currentEnemies.push_back(enemy);
			TMsgSpawnerCheckin checkin;
			checkin.spawnerHandle = CHandle(this).getOwner();
			((CEntity*)enemy)->sendMsg(checkin);
			_spawnTimer = _spawnDelay;
		}
		else {
			_spawnTimer -= dt;
		}
		
	}
}

void TCompEnemySpawnerSpecialTrap::setSpawnDelay(float newtime) {
	_spawnDelay = newtime;
}
