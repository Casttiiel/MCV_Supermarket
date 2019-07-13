#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

class TCompSpawner : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();

private:
	bool _isEnabled = false;
	float _spawnDelay = 3.f;
	float _spawnTimer = _spawnDelay;
	std::string _prefab = PREFAB_CUPCAKE;

	void onTMsgSetActive(const TMsgSetActive & msg);
};

