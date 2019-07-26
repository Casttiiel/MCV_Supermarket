#include "mcv_platform.h"
#include "engine.h"
#include "comp_wind_trap.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/vfx/comp_air.h"
#include "components/ai/others/self_destroy.h"

DECL_OBJ_MANAGER("comp_wind_trap", TCompWindTrap);

void TCompWindTrap::debugInMenu() {
}

void TCompWindTrap::load(const json& j, TEntityParseContext& ctx) {
}

void TCompWindTrap::registerMsgs() {
	DECL_MSG(TCompWindTrap, TMsgEntityTriggerEnter, enable);
	DECL_MSG(TCompWindTrap, TMsgEntityTriggerExit, disable);
	DECL_MSG(TCompWindTrap, TMsgGravity, onBattery);
	DECL_MSG(TCompWindTrap, TMsgEntityCreated, onCreate);
}

void TCompWindTrap::onCreate(const TMsgEntityCreated & msg) {
	TCompCollider* c_col = get<TCompCollider>();
	physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);
	c_trans = get<TCompTransform>();
	physx::PxBoxGeometry box;
	physx::PxShape* colShape;
	rigid_dynamic->getShapes(&colShape,1, 0);
	colShape->getBoxGeometry(box);
	VEC3 creationWindOffset = VEC3(c_trans->getPosition().x, (c_trans->getPosition().y - (box.halfExtents.y)), c_trans->getPosition().z - (box.halfExtents.z));
	//c_col->setGlobalPose(creationWindOffset, QUAT());
	
}

void TCompWindTrap::enable(const TMsgEntityTriggerEnter & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = true;
		player = msg.h_entity;
		dbg("Enemy spawner enabled.\n");
	}
}

void TCompWindTrap::disable(const TMsgEntityTriggerExit & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = false;
		dbg("Enemy spawner disabled.\n");
	}
}

void TCompWindTrap::onBattery(const TMsgGravity & msg) {
	_isEnabled = false;
	//Animate or start particle system, do something
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
}

void TCompWindTrap::update(float dt) {
  if (!player.isValid()) {
    return;
  }
		
	if (_isEnabled) {
		c_trans = get<TCompTransform>();
		VEC3 force = c_trans->getFront();
		force *= windForce * dt;
		CEntity* player_e = (CEntity*)player;
		TMsgTrapWind msg;
		msg.senderType = EntityType::ENVIRONMENT;
		msg.targetType = EntityType::ALL;
		msg.position = c_trans->getPosition();
		msg.intensityDamage = 0.f;
		msg.impactForce = windForce * dt;
		player_e->sendMsg(msg);
	}

  generateWind(dt);
}

void TCompWindTrap::generateWind(float dt) {
  _windCooldownTimer += dt;
  if (_windCooldownTimer >= _windDelay) {
    _windCooldownTimer = 0.0f;
    
    TCompTransform* c_trans = get<TCompTransform>();
    TEntityParseContext ctx;
    ctx.root_transform = *c_trans;
    //change scale randomly
    ctx.root_transform.setScale( 1.0f + randomFloat(-_scaleVar, _scaleVar));
    //change pitch a bit randomly
    float yaw, pitch;
    ctx.root_transform.getAngles(&yaw, &pitch);
    float rollOffset = randomFloat(-_rollVar, _rollVar);
    ctx.root_transform.setAngles(yaw, pitch, rollOffset);
    //change position a bit randomly
    VEC3 offset = VEC3(randomFloat(-radius, radius), randomFloat(0.5f - radius, 0.5f + radius), randomFloat(-radius, radius));
    ctx.root_transform.setPosition(ctx.root_transform.getPosition() + offset);
    
    parseScene("data/prefabs/vfx/air.json", ctx);

    //change speed a bit randomly
    CEntity* e = ctx.entities_loaded[0];
    TCompAir* c_a = e->get<TCompAir>();
    c_a->speed = randomFloat(c_a->speed -_speedVar, c_a->speed + _speedVar);
    TCompSelfDestroy* c_sd = e->get<TCompSelfDestroy>();
    c_sd->setDelay(randomFloat(1.5f, 2.0f));
    c_sd->enable();
  }
}