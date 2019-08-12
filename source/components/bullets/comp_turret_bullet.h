#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/common_msgs.h"
#include "components/common/physics/comp_collider.h"

class TCompTurretBulletController : public TCompBase {

	float speed = 200;
	VEC3  front;
	float mass = 1.5;
	float collision_radius = 2.f;
	CHandle h_sender;
	float intensityDamage = 20.0f;
	float _explosionForce = 5.f;
	float _explosionRadius = 3.f;
	float life_time = 50.0f;
	bool impact = false;

	CHandle e_player;
	
	DECL_SIBLING_ACCESS();
	void onBulletInfoMsg(const TMsgAssignBulletOwner& msg);
  void onCollision(const TMsgOnContact& msg);
  void explode();

public:
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
};