#include "mcv_platform.h"
#include "engine.h"
#include "comp_enemy_spawner.h"
#include "components/common/comp_transform.h"
#include "components/ai/others/comp_blackboard.h"
#include "components/animation/comp_prop_animation.h"
#include "components/ai/bt/bt_cupcake.h"

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
  is_destroyed = true;
  //Animate or start particle system, do something
	/*CHandle(this).getOwner().destroy();
	CHandle(this).destroy();*/
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
	if (working && !is_destroyed) {
		if (_isEnabled) {
			if (_spawnTimer <= 0 && _currentEnemies.size() < _spawnMaxNumber) {
				dbg("Spawning %s\n", _prefab);
				TCompPropAnimator* animator = get<TCompPropAnimator>();
				animator->playAnimation(TCompPropAnimator::OVEN_OPEN, 1.f);
				TCompTransform* c_trans = get<TCompTransform>();
				VEC3 spawnPoint = c_trans->getPosition() + VEC3(0,1.5,0) +  (c_trans->getFront() * _spawnOffset);;
				CHandle enemy = GameController.spawnPrefab(_prefab, spawnPoint);
				
				//rotamos el cupcake hacia la posicion del jugador al spawnear para que te vea siempre
				CEntity* player = GameController.getPlayerHandle();
				CEntity* e_enemy = (CEntity*)enemy;
				TCompTransform* c_trans_enemy = e_enemy->get<TCompTransform>();
				if(player != nullptr) {
					TCompTransform* c_trans_player = player->get<TCompTransform>();
					c_trans_enemy->rotateTowards(c_trans_player->getPosition());
				}

				if (_prefab == PREFAB_CUPCAKE_) {
					CBTCupcake* cbt = e_enemy->get<CBTCupcake>();
					cbt->setLife(lifePrefabSpawner);
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
}

void TCompEnemySpawner::setLifeSpawner(float new_value) {
	lifePrefabSpawner = new_value;
}
