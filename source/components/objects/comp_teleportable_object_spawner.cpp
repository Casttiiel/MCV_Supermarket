#include "mcv_platform.h"
#include "engine.h"
#include "components/common/comp_transform.h"
#include "comp_teleportable_object_spawner.h"
#include "components/ai/others/self_destroy.h"

DECL_OBJ_MANAGER("comp_teleportable_object_spawner", TCompTeleportableObjectSpawner);

void TCompTeleportableObjectSpawner::debugInMenu() {
}

void TCompTeleportableObjectSpawner::load(const json& j, TEntityParseContext& ctx) {
	_spawnDelay = j.value("_spawnDelay", _spawnDelay);
	_prefab = j.value("_prefab", _prefab);
	_spawnerRange = j.value("_spawnerRange", _spawnerRange);
}

void TCompTeleportableObjectSpawner::registerMsgs() {
	DECL_MSG(TCompTeleportableObjectSpawner, TMsgEntityTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTeleportableObjectSpawner, TMsgEntityTriggerExit, onTriggerExit);
	//DECL_MSG(TCompTeleportableObjectSpawner, TMsgEntityCreated, onCreate);
}

void TCompTeleportableObjectSpawner::onCreate(const TMsgEntityCreated & msg) {
	/*TCompTransform* c_trans = get<TCompTransform>();
	_objectHandle = GameController.spawnPrefab(_prefab, c_trans->getPosition());
	if (_objectHandle.isValid()) {
		_isEnabled = false;
	}*/
}


void TCompTeleportableObjectSpawner::onTriggerEnter(const TMsgEntityTriggerEnter & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		CEntity* objectEntity = (CEntity*)_objectHandle;
		TCompTransform* object_trans = objectEntity->get<TCompTransform>();
		TCompTransform* c_trans = get<TCompTransform>();

		CEntity* playerEntity = (CEntity*) GameController.getPlayerHandle();
		TCompTransform* player_trans = playerEntity->get<TCompTransform>();
		if  (VEC3().Distance(c_trans->getPosition(), object_trans->getPosition()) > _spawnerRange) {
			//dbg("Player entered the teleportable trigger,\n and the object is far enough to start selfdestruct sequence.\n");
			//dbg("Distance:%f\n", VEC3::Distance(player_trans->getPosition(), object_trans->getPosition()));
			//Enable entity self destrcut
			CEntity* objectEntity = (CEntity*)_objectHandle;
			TCompSelfDestroy* c_sd = objectEntity->get<TCompSelfDestroy>();
			c_sd->enable();
		}
		//Disable spawner while the player is inside
		_isEnabled = false;
		_spawnTimer = _spawnDelay;
	}
}

void TCompTeleportableObjectSpawner::onTriggerExit(const TMsgEntityTriggerExit & msg) {
	//if the entity is the object
	if (msg.h_entity == _objectHandle) {
		//dbg("Teleportable object exited the trigger.\nTelportable_object_spawner activated\n");
		//Enable spawn timer
		_isEnabled = true;
		_spawnTimer = _spawnDelay;
		//Enable entity self destrcut
		CEntity* objectEntity = (CEntity*)msg.h_entity;
		TCompSelfDestroy* c_sd = objectEntity->get<TCompSelfDestroy>();
		c_sd->enable();
		
	}
	//if the entity is not the object and the object is valid
	else if(_objectHandle.isValid()){
		TCompTransform* object_trans = ((CEntity*)_objectHandle)->get<TCompTransform>();
		TCompTransform* c_trans = get<TCompTransform>();
		//if the entity is the player and
		//the spawner is not enabled and
		//the object is far enough
		if (msg.h_entity == GameController.getPlayerHandle() && !_isEnabled && VEC3().Distance(c_trans->getPosition(), object_trans->getPosition()) > _spawnerRange) {
			//dbg("Player exited the trigger while the spawner was not enabled and the object was far enough.\nTelportable_object_spawner activated\n");
			_isEnabled = true;
			_spawnTimer = _spawnDelay;
		}
	}
	//if the entity is not the object and the object is not valid
	else {
		//dbg("Player exited the trigger while the spawner was not enabled and the object was far enough.\nTelportable_object_spawner activated\n");
		_isEnabled = true;
		_spawnTimer = _spawnDelay;
	}
}
void TCompTeleportableObjectSpawner::update(float dt) {
	if (_isEnabled) {
		if (_spawnTimer <= 0) {
			//dbg("Spawning %s\n", _prefab.c_str());
			TCompTransform* c_trans = get<TCompTransform>();
			_objectHandle = GameController.spawnPrefab(_prefab, c_trans->getPosition());
			_isEnabled = false;
			_spawnTimer = _spawnDelay;
			//dbg("Teleportable object spawner disabled.\n");
		}
		else {
			_spawnTimer -= dt;
		}
	}
}
