#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

class TCompRemoteTrigger : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();
 

private:
  void onEnter(const TMsgEntityTriggerEnter & msg);

  std::vector<CEntity*> entities;
  std::vector<bool> actions;
};

