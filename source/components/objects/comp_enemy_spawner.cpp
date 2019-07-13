#include "mcv_platform.h"
#include "engine.h"
#include "comp_enemy_spawner.h"
#include "components/common/comp_transform.h"
#include "components/ai/others/comp_blackboard.h"
#include "components/animation/comp_prop_animation.h"

DECL_OBJ_MANAGER("comp_enemy_spawner", TCompEnemySpawner);

void TCompEnemySpawner::debugInMenu() {



}

void TCompEnemySpawner::load(const json& j, TEntityParseContext& ctx) {
	_prefab = j.value("_prefab", _prefab);
	_spawnMaxNumber = j.value("_spawnMaxNumber", _spawnMaxNumber);
	working = j.value("working", working);

}

void TCompEnemySpawner::registerMsgs() {
	DECL_MSG(TCompEnemySpawner, TMsgEntityTriggerEnter, enable);
	DECL_MSG(TCompEnemySpawner, TMsgEntityTriggerExit, disable);
	DECL_MSG(TCompEnemySpawner, TMsgGravity, onBattery);
	DECL_MSG(TCompEnemySpawner, TMsgSpawnerCheckout, onCheckout);
}

void TCompEnemySpawner::enable(const TMsgEntityTriggerEnter & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = true;
		dbg("Enemy spawner enabled.\n");
	}
}

void TCompEnemySpawner::disable(const TMsgEntityTriggerExit & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = false;
		dbg("Enemy spawner disabled.\n");
	}
}

void TCompEnemySpawner::onBattery(const TMsgGravity & msg) {
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

void TCompEnemySpawner::onCheckout(const TMsgSpawnerCheckout & msg) {
	for (auto it = _currentEnemies.begin(); it != _currentEnemies.end(); ++it) {
		if (*it == msg.enemyHandle) {
			_currentEnemies.erase(it);
			_spawnTimer = _spawnDelay;
			return;
		}
	}
}

void TCompEnemySpawner::update(float dt) {
	if (working) {
		if (_isEnabled) {
			if (_spawnTimer <= 0 && _currentEnemies.size() < _spawnMaxNumber) {
				dbg("Spawning %s\n", _prefab);
        TCompPropAnimator* animator = get<TCompPropAnimator>();
        animator->playAnimation(TCompPropAnimator::OVEN_OPEN, 1.f);
				TCompTransform* c_trans = get<TCompTransform>();
        VEC3 spawnPoint = c_trans->getPosition() + (c_trans->getFront() * _spawnOffset);
				CHandle enemy = GameController.spawnPrefab(_prefab, spawnPoint);
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

	
}
