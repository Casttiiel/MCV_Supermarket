#ifndef _BT_GOLEM
#define _BT_GOLEM

#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "bt_controller.h"


class CBTGolem : public BTController {
	CHandle  h_player;
	DECL_SIBLING_ACCESS();
public:
	void create(string); //init

	 //acciones == hojas
	int actionIdle();
	int actionAlign();
	int actionChargingThrow();
	int actionThrow();
	int actionMelee();
	int actionDeath();
	int actionImpactReceived();
	int actionGravityReceived();
	int actionChargingMelee();

	template <typename T>
	void Send_DamageMessage(CEntity* entity, float dmg);

	static void registerMsgs();


	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	void renderDebug();
	//condiciones para establecer prioridades
	bool conditionView();
	bool conditionDistanceThrow();
	bool conditionDistanceMelee();
	bool conditionDeath();
	bool conditionImpactReceived();
	bool conditionGravityReceived();
	bool conditionTimerThrow();
	bool conditionTimerMelee();
private:
	float length_cone = 30.0f;
	float half_cone = 180.0f;
	float cone_vision = 10.f;

	float forgetEnemyRange = 3.0;
	float forgetEnemy = length_cone + forgetEnemyRange;

	float twistSpeed = 5.f;
	float meleeRange = 4.0f;
	float impactForceAttack = 8.0f;

	float throwFrequecy = 1.5f;
	float timerGrenade = throwFrequecy;

	float meleeFrequency = 1.0f;
	float timerMelee = meleeFrequency;
	float hearing_radius = 2.0f;
	float damageArea = 1.0;
	float life = 100.f;

	float epsilonMelee = 7.f;
	float epsilonThrow = 15.f;
	float epsilon = 7.f;

	float battery_time = 0.f;

	bool damageaded = false;

	int intensityDamage = 0;

	int damage = 10;

	void onCollision(const TMsgOnContact& msg);
	void onDamageInfoMsg(const TMsgDamage& msg);
	void onGravity(const TMsgGravity& msg);
	CHandle h_sender;
	CHandle h_bullet;
};



#endif _BT_GOLEM