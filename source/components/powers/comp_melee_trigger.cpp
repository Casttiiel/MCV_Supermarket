#include "mcv_platform.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "components/ai/others/self_destroy.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "components/common/comp_tags.h"
#include "components/common/physics/comp_collider.h"
#include "input/module_input.h"
#include "modules/module_physics.h"
#include "components/controllers/comp_parabolic_launch.h"
#include "comp_melee_trigger.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_melee_trigger", TCompMeleeTrigger);
void TCompMeleeTrigger::debugInMenu() {
}

void TCompMeleeTrigger::load(const json& j, TEntityParseContext& ctx) {
}

void TCompMeleeTrigger::registerMsgs() {
    DECL_MSG(TCompMeleeTrigger, TMsgEntityTriggerEnter, onCollision);
    DECL_MSG(TCompMeleeTrigger, TMsgEntityTriggerExit, onCollisionOut);
}


void TCompMeleeTrigger::onCollision(const TMsgEntityTriggerEnter& msg) {
  CEntity* player = getEntityByName("Player");
  TCompCharacterController* c_c = player->get<TCompCharacterController>();
  if (c_c == nullptr)
    return;
  bool attacking = c_c->getAttacking();
 
  if (attacking) {
    TMsgMeleeHit meleeHit;
    meleeHit.h_entity = msg.h_entity;
    player->sendMsg(meleeHit);
  }
  else {
    _currentEnemies.push_back(msg.h_entity);
  }
}

void TCompMeleeTrigger::onCollisionOut(const TMsgEntityTriggerExit& msg) {
  for (int i = 0; i < _currentEnemies.size(); i++) {
    if (msg.h_entity == _currentEnemies[i]) {
      _currentEnemies.erase(_currentEnemies.begin() + i);
      return;
    }
  }
}


void TCompMeleeTrigger::update(float delta) {
  CEntity* player = getEntityByName("Player");
  TCompCharacterController* c_c = player->get<TCompCharacterController>();
  if (c_c == nullptr)
    return;
  bool attacking = c_c->getAttacking();
  if (attacking != before && attacking) { // si empiezas a atacar
    
    for (int i = 0; i < _currentEnemies.size(); i++) {
      TMsgMeleeHit meleeHit;
      meleeHit.h_entity = _currentEnemies[i];
      if(meleeHit.h_entity.isValid())
        player->sendMsg(meleeHit);
    }
  }

  before = attacking;
}

void TCompMeleeTrigger::renderDebug() {
}
