#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

class TCompTeleportableObjectSpawner : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();

private:
	CHandle _objectHandle;
	bool _isEnabled = true;
	float _spawnerRange = 2.f;
	float _spawnDelay = 5.f;
	float _spawnTimer = _spawnDelay;
	std::string _prefab = PREFAB_TELEPORTABLE_OBJECT;

	void onTriggerEnter(const TMsgEntityTriggerEnter & msg);
	void onTriggerExit(const TMsgEntityTriggerExit & msg);
	void onCreate(const TMsgEntityCreated & msg);
};

