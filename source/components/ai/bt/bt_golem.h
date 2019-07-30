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
	int nothing();
	int actionAlign();
	int actionChargingThrow();
	int actionThrowCupcake();
	int actionThrow(); 
	int actionThrowCookieSimple();
	int actionThrowCookieSpread();
	int actionMelee();
	int actionDeath();
	int actionImpactReceived();
	int actionGravityReceived();
	int actionChargingMelee();
	int actionMeleeCinematic();
	void singleShot();
	void spreadShot();

	template <typename T>
	void Send_DamageMessage(CEntity* entity, float dmg);

	static void registerMsgs();


	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	void renderDebug();
	//condiciones para establecer prioridades
	bool conditionView();
	bool conditionCinematic();
	bool conditionDistanceThrow();
	bool conditionDistanceMelee();
	bool conditionDeath();
	bool conditionImpactReceived();
	bool conditionGravityReceived();
	bool conditionTimerThrow();
	bool conditionTimerMelee();
	bool conditionRandomThrowCupcake();
	bool conditionRandomThrowParabolic();
	bool conditionRandomThrowSimpleCookie();

	bool conditionAttackCinematic();


private:



	//range values
	float forgetEnemyRange = 3.0;
	float forgetEnemy = length_cone + forgetEnemyRange;
	float meleeRange = 4.0f;
	float length_cone = 30.0f;
	float half_cone = 180.0f;
	float cone_vision = 10.f;
	float viewDistance = 40.f;
	float hearing_radius = 2.0f;
	float damageArea = 1.0; //melee attack
	//end range values

	//speed values
	float twistSpeed = 5.f;
	//end speed values

	//frequency values
	float throwFrequecy = 1.0f; //cada cuanto reevalua que tipo de disparo hará
	float timerGrenade = throwFrequecy;

	float meleeFrequency = 1.0f;
	float timerMelee = meleeFrequency;

	float _burstDelay = .5f;
	float _burstTimer = 0.f;
	//end frequency values


	//probability values
	float throwCupcakeProbability = 100; //10%
	float throwParabolicProjectileProb = 0; //en realidad es 25 - 10 que es 15%
	float throwSimpleFortuneCookieProb = 75; // en realidad es 75 - 25 que es 50%
	//el otro 25 es para el lanzamiento triple, que no necesita variable-- seria un 25%
	//end probability values

	//damage values
	int intensityDamage = 0;
	float meleeDamage = 10;
	int damage = 10;
	float _bulletDamage = 1.f;
	float _bulletForce = 2.5f;
	float impactForceAttack = 8.0f;
	//end damage values

	//GAMEPLAY VALUES
	float life = 100.f;
	int numberOfCookiesSimple = 5; //number of burst in simple cookie
	int numberOfCookiesTriple = 3; //number of burst in triple coockie
	int _spawnMaxNumber = 30; //number of cupcakes
	float battery_time = 0.f;
	//end gameplay values

	//others
	int randomNumber = 0;
	float impactForceCinematic = 25.0f;
	float golem_height = 13.0f;
	bool attackCinematic = false;
	int _shotsFired = 0;

	float epsilonMelee = 7.f;
	float epsilonThrow = 15.f;
	float epsilon = 7.f;

	bool damageaded = false;
	float _playerOffset = 7.5f;

	bool scriptedGolem = false; //TODO: PONER A TRUE
	bool golemCinematic = false;
	bool inCinematic = false;

	bool firstExec= true;
	float projectileDelay = 0.83;
	float delay = projectileDelay;
	float delayCupcake = 0.8;
	float throwActive = false;
	int throwType = 1; //1 cupcake ; 2 parabolic



	std::vector<CHandle> _currentEnemies;

	void onCollision(const TMsgOnContact& msg);
	void onDamageInfoMsg(const TMsgDamage& msg);
	void onGravity(const TMsgGravity& msg);
	void onCheckout(const TMsgSpawnerCheckout & msg);
	void onBornChild(const TMsgSpawnerFather & msg);
	void activeGolem(const TMsgActiveGolem & msg);
	void onCinematic(const TMsgOnCinematic & msg);

	void updateBT();

	bool isView();
	CHandle h_sender;
	CHandle h_bullet;
};



#endif _BT_GOLEM