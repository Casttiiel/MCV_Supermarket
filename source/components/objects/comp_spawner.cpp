#include "mcv_platform.h"
#include "engine.h"
#include "comp_spawner.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("comp_spawner", TCompSpawner);

void TCompSpawner::debugInMenu() {
}

void TCompSpawner::load(const json& j, TEntityParseContext& ctx) {
	_prefab = j.value("_prefab", _prefab);
	_spawnDelay = j.value("_spawnDelay", _spawnDelay);
}

void TCompSpawner::registerMsgs() {
	DECL_MSG(TCompSpawner, TMsgSetActive, onTMsgSetActive);
}

void TCompSpawner::onTMsgSetActive(const TMsgSetActive & msg) {
	_isEnabled = msg.active;
	dbg("Spawner %s\n", _isEnabled ? "activates" : "deactivates");
}

void TCompSpawner::update(float dt) {
	if (_isEnabled) {
		if (_spawnTimer <= 0) {
			dbg("Spawning %s\n", _prefab);
			TCompTransform* c_trans = get<TCompTransform>();
			GameController.spawnPrefab(_prefab, c_trans->getPosition());
			_spawnTimer = _spawnDelay;
		}
		else {
			_spawnTimer -= dt;
		}
	}
}
