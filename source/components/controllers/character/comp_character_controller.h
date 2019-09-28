#pragma once

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/ai/graph/ai_controller.h"
#include "components/controllers/comp_sCart_controller.h"
#include "components/animation/comp_player_animation.h"
#include "modules/game/audio/audioEvent.h"
#include "components/controllers/comp_inventory.h"

class TCompCharacterController : public IAIController
{
public:
  VEC3 posCkeckpoint = VEC3(0, 0, 0); //cambiar posicion cuando Sepamos la posicion de inicio del jugador
  float maxLife = 100.f;
  float life = maxLife;
  bool endGame = false;
  PowerType power_selected = PowerType::TELEPORT;

  bool aiming = false;
  bool is_grounded = false;
  void Init();
	void update(float dt);
  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
  static void registerMsgs();

  void setMeleeMultiplier(float newMulti);
  void setSpeed(float newSpeed);
  void setDashSpeed(float dashSpeed) { dash_speed = dashSpeed; }
  float getBaseSpeed();

  //Shopping Cart
  void dismount();
  //End Shopping Cart

  //Mesh Swap
  void SwapMesh(int state);
  //End Mesh Swap
  bool enabled = true;
  //POWER UPS
  void  applyPowerUp(float quantity, PowerUpType type, float extraBarSize);
  void heal();
  void healPartially(float health);
  //madness
  void restoreMadness();
  float getMaxMadness();
  bool getIsMounted() { return isMounted; }
  VEC3 getMovementDirection() { return movementDirection; }

  /*bool unLockableBattery = false;
  bool unLockableTeleport = false;
  bool unLockableChilli = false;
  bool unLockableCoffe = false;*/
  bool cinematic = false;
private:
    VEC3 movementDirection = VEC3().Zero;
	int typeRender = 2;
  //Shopping Cart
  bool isMounted = false;
  float interactRange = 3.5f;
  void interact();
  void mount(CHandle vehicle);
  //End Shopping Cart
  std::string statePrevious = "";
  float distance_to_aim = 20.0f;
  float speed = 7.f;
  
  float base_speed = speed;
  float rotation_speed = 4.0f;
  float dash_speed = 35.0f;
  float dash_limit = 0.1f;
  float dash = dash_limit;
  float time_between_dashes = 1.5f;
  float time_to_next_dash = 0.0f;
  float jump_force = 7.f;
  float double_jump_force = 4.f;
  float falling_factor_hover = 0.25f;
  float moving_factor_hover = 1.0f;
  float damaged_force = 5.0f;
  bool can_double_jump = true;
  bool enemy_in_range = true;
  bool animation1Done = false;
  bool isBatteryAlive = false;

	float invulnerabilityTimer = 0.f;
	float invulnerabilityTimeDuration = 1.0;

  //Audio Values
    AudioEvent damagedAudio;
    AudioEvent footSteps;
  //Audio Values

  //Melee Values
  bool alreadyAttacked = false;
  bool attackFirstExecution = true;
  float meleeTimer = 0.f;
  float meleeDelay = 0.5f;
  float meleeTotalDuration = 0.4f;
  float meleeCurrentDuration = 0.f;
  float meleeDistance = 1.5f;
  float meleeDamage = 30.f;
  float meleeRadius = 2.5f;
  std::vector<CHandle> entitiesHit;
  float impactForceAttack = 8.0f;
  //End Melee Values

  //ChargedAttack Values
  float chargedAttack_chargeDelay = 1.f;
  float chargedAttack_damage = 20.f;
  float chargedAttack_radius = 5.f;
	float chargedAttack_impactForce = 20.0f;
  float chargedAttack_buttonPressThreshold = 0.2f;
  float chargedAttack_buttonPressTimer = 0.f;
  float chargedAttack_playerSpeed = base_speed * 0.5;
  bool chargedAttack_releasing = false;
  bool chargedAttack_onAir = false;
  //End ChargedAttack Values

  //chilli values
  float hxChilli = 1.5f;
  float hyChilli = 1.f;
  float hzChilli = 2.f;
  float chilliDistance = 3.f;

  float rumble_time = 0.0f;
  
  bool cinematicFinish = false;

  float speedCinematicSpecial = 3.f;
  VEC3 targetTower = VEC3(-103, -0.057, -211);

  CHandle h_camera;

  //STATES
  void grounded(float delta);
  void dashing(float delta);
  void onAir(float delta);
  void damaged(float delta);
  void dead(float delta);
  void win(float delta);
  void noclip(float delta);
  void idleCinematic(float delta);
  void specialCinematic(float delta);

   //UTILS
  void treatRumble(float delta);
  void getInputForce(VEC3 &dir);
  void rotatePlayer(const VEC3 &dir, float delta, bool start_dash = false);
  bool isGrounded();
  void powerSelection();
  void shoot();
  void attack(float delta);
  void chargedAttack(float delta);

   //EVENTS
  void onCollision(const TMsgOnContact& msg);
  void onEnter(const TMsgEntityTriggerEnter& trigger_enter);
  void onDamageAll(const TMsgDamageToAll& msg);
 // void onDamage(const TMsgDamage& msg);
  void onGenericDamage(const TMsgDamage& msg);
  void onPowerUp(const TMsgPowerUp& msg);
  void onBatteryDeactivation(const TMsgBatteryDeactivates& msg);
  void onAnimationFinish(const TCompPlayerAnimator::TMsgPlayerAnimationFinished& msg);
  void onCinematic(const TMsgOnCinematic& msg);
  void onCinematicSpecial(const TMsgOnCinematicSpecial& msg);
  void onTrapWind(const TMsgTrapWind& msg);
  void onTriggerFalloutDead(const TMSgTriggerFalloutDead& msg);


  //?????
  void mounted(float delta);


  DECL_SIBLING_ACCESS();
};

