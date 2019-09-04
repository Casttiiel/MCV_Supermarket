#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

class TCompEnemySpawner : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();

private:
	bool working = true; //temporal :)
	bool _isEnabled = false;
  bool is_destroyed = false;
	int _spawnMaxNumber = 2;
	std::vector<CHandle> _currentEnemies;
	float _spawnDelay = 3.f;
	float _spawnTimer = _spawnDelay;
	std::string _prefab = PREFAB_CUPCAKE;
  float _spawnOffset = 2.5f;

	void enable(const TMsgEntityTriggerEnter & msg);
	void disable(const TMsgEntityTriggerExit & msg);
	void onBattery(const TMsgGravity & msg);
	void onCheckout(const TMsgSpawnerCheckout & msg);
};

