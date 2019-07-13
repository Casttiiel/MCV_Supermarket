#include "mcv_platform.h"
#include "ai_wind_effect.h"
#include "engine.h"
#include "entity/entity.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/common/comp_tags.h"
#include "time.h"
#include "stdlib.h"
#include "modules/module_physics.h"
#include "components/powers/comp_battery.h"


using namespace physx;


DECL_OBJ_MANAGER("comp_wind_effect", TCompWindEffect);

void TCompWindEffect::Init()
{
	AddState("ON_STATE", (statehandler)&TCompWindEffect::OnState);
	AddState("OFF_STATE", (statehandler)&TCompWindEffect::OffState);
	AddState("DEAD_STATE", (statehandler)&TCompWindEffect::DeadState);

	ChangeState("ON_STATE");
}

void TCompWindEffect::OnState(float dt)
{
	TCompTransform* c_trans = get<TCompTransform>();
	TCompRender* c_render = get<TCompRender>();
	c_render->is_visible = true;
	c_render->updateRenderManager();
	activateWind = true;
	if (activateWind && inWind) {
		//enviar mensaje de da�o por viento
		TMsgDamage msg;
		
		msg.senderType = EntityType::ENVIRONMENT;
		msg.targetType = EntityType::PLAYER;
		msg.position = c_trans->getPosition();
		msg.intensityDamage = _windDamage;
		msg.impactForce = 300.f * dt; //0.8f
		CHandle hitCollider;
		hitCollider.fromVoidPtr(c_col->actor->userData);
		if (hitCollider.isValid()) {
			//Send the message we've been given
			CEntity* entityContact =  getEntityByName("Player");
			entityContact->sendMsg(msg);
		}

		//GameController.generateDamageSphere(c_trans->getPosition(), _explosionRadius, msg, "VulnerableToExplosions");
	}
	_durationOn -= dt;
	if (_durationOn <= 0) {
		_durationOn = _durationOn_init;
		ChangeState("OFF_STATE");
	}
}

void TCompWindEffect::OffState(float dt)
{
	//dbg("En estado OFF\n");
	TCompRender* c_render = get<TCompRender>();
	c_render->is_visible = false;
	c_render->updateRenderManager();
	activateWind = false;
	_durationOff -= dt;



	if (_durationOff <= 0) {
		_durationOff = _durationOff_init;
		ChangeState("ON_STATE");
	}
	
}



void TCompWindEffect::DeadState(float dt) {
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
}



void TCompWindEffect::onWindInfoMsg(const TMsgAssignBulletOwner& msg) {

	c_trans = get<TCompTransform>();
	c_col = get<TCompCollider>();
	c_handle = msg.h_owner;
	CEntity* e_player = (CEntity *) c_handle;
	TCompWind* compW = e_player->get<TCompWind>();
	_durationOff = compW->_durationOff;
	_durationOff_init = compW->_durationOff_init;
	_durationOn = compW->_durationOn;
	_durationOn_init = compW->_durationOn_init;
	c_trans->setPosition(msg.source);
	c_col->setGlobalPose(msg.source,QUAT());
}


void TCompWindEffect::registerMsgs() {
	DECL_MSG(TCompWindEffect, TMsgAssignBulletOwner, onWindInfoMsg);
	DECL_MSG(TCompWindEffect, TMsgDamage, onDamageInfoMsg);
	DECL_MSG(TCompWindEffect, TMsgEntityTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompWindEffect, TMsgEntityTriggerExit, onTriggerExit);
	DECL_MSG(TCompWindEffect, TMsgGravity, onBattery);
}


void TCompWindEffect::onDamageInfoMsg(const TMsgDamage& msg) {
}

void TCompWindEffect::onTriggerEnter(const TMsgEntityTriggerEnter& msg) {
	inWind = true;
	/*
	CHandle handle = msg.h_entity;
	CEntity* e_entity = (CEntity *) handle;
	e_entity->get<TCompBatteryController>();
	if (e_entity != nullptr) {
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
	*/
	
		
	
	
}

void TCompWindEffect::onTriggerExit(const TMsgEntityTriggerExit& msg) {
	inWind = false;
}



void TCompWindEffect::debugInMenu() {

	
}

void TCompWindEffect::onBattery(const TMsgGravity & msg) {
	//Animate or start particle system, do something
	ChangeState("DEAD_STATE");
}






void TCompWindEffect::load(const json& j, TEntityParseContext& ctx) {
	/*_durationOn = j.value("_durationOn", _durationOn);
	_durationOn_init = j.value("_durationOn_init", _durationOn_init);
	_durationOff = j.value("_durationOff", _durationOff);
	_durationOff_init = j.value("_durationOff_init", _durationOff_init);
	_windDamage = j.value("_windDamage", _windDamage);*/
	this->Init();
	

}

