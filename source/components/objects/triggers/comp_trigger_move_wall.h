#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

class TCompTriggerMoveWall : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();
 

private:
  void onEnter(const TMsgEntityTriggerEnter & msg);
  void onExit(const TMsgEntityTriggerExit & msg);
  //void onActiveMsg(const TMsgSetActive & msg);

  std::vector<CEntity*> elementsToMove;
  bool destroy= false;

};

