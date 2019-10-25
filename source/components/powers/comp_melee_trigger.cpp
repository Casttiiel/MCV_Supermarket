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
    EngineAudio.playEvent("event:/Character/Attacks/Melee_Hit");
    expansiveWave();
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
    if (_currentEnemies.size() > 0) {
      EngineAudio.playEvent("event:/Character/Attacks/Melee_Hit");
      expansiveWave();
    }
  }

  before = attacking;
}

void TCompMeleeTrigger::renderDebug() {
}

void TCompMeleeTrigger::expansiveWave() {
  CEntity* player = getEntityByName("Player");
  TCompTransform* c_trans = player->get<TCompTransform>();

  CEntity* onom_manager = getEntityByName("Onomatopoeia Particles");
  TMsgOnomPet msgonom;
  msgonom.type = 1.0f;
  msgonom.pos = c_trans->getPosition();
  onom_manager->sendMsg(msgonom);


  PxVec3 pos = VEC3_TO_PXVEC3(c_trans->getPosition());
  PxQuat ori = QUAT_TO_PXQUAT(c_trans->getRotation());
  PxSphereGeometry geometry(5.0f);
  const PxU32 bufferSize = 256;
  PxOverlapHit hitBuffer[bufferSize];
  PxOverlapBuffer buf(hitBuffer, bufferSize);
  PxTransform shapePose = PxTransform(pos, ori);
  PxQueryFilterData filter_data = PxQueryFilterData();
  filter_data.data.word0 = EnginePhysics.Product;
  bool res = EnginePhysics.gScene->overlap(geometry, shapePose, buf, filter_data);
  if (res) {
    for (PxU32 i = 0; i < buf.nbTouches; i++) {
      CHandle h_comp_physics;
      h_comp_physics.fromVoidPtr(buf.getAnyHit(i).actor->userData);
      CEntity* entityContact = h_comp_physics.getOwner();
      if (entityContact) {
        TMsgDamage msg;
        // Who sent this bullet
        msg.h_sender = CHandle(this).getOwner();
        msg.h_bullet = CHandle(this).getOwner();
        msg.position = c_trans->getPosition() + VEC3::Up;
        msg.senderType = PLAYER;
        msg.intensityDamage = 5.0f;
        msg.impactForce = 5.0f;
        msg.damageType = MELEE;
        msg.targetType = ENEMIES;
        entityContact->sendMsg(msg);
      }
    }
  }
}
