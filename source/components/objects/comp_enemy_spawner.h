#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "modules/game/module_game_manager.h"


class TCompEnemySpawner : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:

   

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void setLifeSpawner(float new_life);
  float getLifeSpawner();
  static void registerMsgs();

  void setScriptTriggerActivate(bool activate);
  void setCurveForSpawner(std::string curveForCupcake);
  void setComportamentNormal(int value);
  void setSpawnDelay(float newtime);
  bool working = true;

 

private:

	#define PREFAB_CUPCAKE_ "data/prefabs/enemies/bt_cupcake.json"

	
	bool _isEnabled = false;
  bool is_destroyed = false;
	int _spawnMaxNumber = 2;
	std::vector<CHandle> _currentEnemies;
	float _spawnDelay = 3.f;
	float _spawnTimer = _spawnDelay;
	std::string _prefab = PREFAB_CUPCAKE_;
    float _spawnOffset = 2.5f;
    float lifePrefabSpawner = 50.f;
	void enable(const TMsgEntityTriggerEnter & msg);
	void disable(const TMsgEntityTriggerExit & msg);
	void onBattery(const TMsgGravity & msg);
	void onCheckout(const TMsgSpawnerCheckout & msg);
	

	int comportamentNormal = 0;

	bool scriptTriggerActivate = false;
	std::string curveForCupcake = "";
	

};

