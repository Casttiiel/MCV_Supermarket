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
}


void TCompMeleeTrigger::onCollision(const TMsgEntityTriggerEnter& msg) {
    if (!_isEnabled)
        return;
    CEntity* player = getEntityByName("Player");
    TMsgMeleeHit meleeHit;
    meleeHit.h_entity = msg.h_entity;
    player->sendMsg(meleeHit);

}


void TCompMeleeTrigger::update(float delta) {
  CEntity* player = getEntityByName("Player");
  TCompCharacterController* c_c = player->get<TCompCharacterController>();
  if (c_c == nullptr)
    return;
  bool attacking = c_c->getAttacking();
  if (attacking != before) {//cambio de estado
    TCompCollider* c_col = get<TCompCollider>();
    PxShape* colShape;
    c_col->actor->getShapes(&colShape, 1, 0);
    if (attacking) {
      colShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    }
    else {
      colShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    }
  }
  before = attacking;
}

void TCompMeleeTrigger::renderDebug() {
}
