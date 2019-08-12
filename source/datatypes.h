#ifndef INC_COMMON_DATATYPES
#define INC_COMMON_DATATYPES

#include "geometry/transform.h"

enum PowerType { 
    MELEE = 0,
    CHARGED_ATTACK,
    FIRE, 
    FIRECOMBO, 
    COFFEE, 
    TELEPORT, 
    BATTERY, 
    PUDDLE, 
    FOUNTAIN 
};
enum PowerUpType { 
    HEALTH_UP = 0, 
    MADNESS_UP, 
    ACTIVATE_CHILLI, 
    ACTIVATE_COFFEE, 
    ACTIVATE_TELEPORT, 
    ACTIVATE_BATTERY };
enum ScriptType {
	HEAL_PLAYER,
	RESTORE_MADNESS,
	GOD_MODE,
	SPAWN,
	SALUTE,
	ACTIVE_ENEMIES,
  ACTIVATE_GAMEPLAY_FRAGMENT,
  DEACTIVATE_GAMEPLAY_FRAGMENT,
  AMBUSH_EVENT,
	ACTIVATE_SCENE,
	ACTIVE_SUSHIS,
	DELETE_GRIETAS
};
enum Events {
	TRIGGER_ENTER,
	TRIGGER_EXIT
};

enum EntityType { 
	PLAYER = 1 << 0,
	CUPCAKE = 1 << 1,
  SUSHI = 1 << 3,
  RANGED_SUSHI = 1 << 4,
	GOLEM = 1 << 5,
	ENVIRONMENT = 1 << 6,
  CUPCAKE_SPAWNER = 1 << 7,
  EXPLOSIVE_OBJECT = 1 << 8,
  DUMPLING_TURRET = 1 << 9,
  THERMOSTATE = 1 << 10,
	SCART = 1 << 11,
	GRENADE_GOLEM = 1 << 12,
  UNDEFINED = 1 << 999,
	ENEMIES = CUPCAKE | SUSHI | GOLEM,
	ALL = PLAYER | ENEMIES
	};

#endif

