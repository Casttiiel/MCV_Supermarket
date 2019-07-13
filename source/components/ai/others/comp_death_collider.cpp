#include "mcv_platform.h"
#include "comp_death_collider.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_tags.h"
#include "components/common/comp_render.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"
#include "modules/module_physics.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_death_collider", TCompDeath);


void TCompDeath::load(const json& j, TEntityParseContext& ctx) {
	damage = j.value("damage", damage);
	damageInterval = j.value("damageInterval", damageInterval);
}

void TCompDeath::registerMsgs() {
	DECL_MSG(TCompDeath, TMsgOnContact, onCollision);
}

void TCompDeath::onCollision(const TMsgOnContact& msg) {

	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
	TCompTags* c_tag = source_of_impact->get<TCompTags>();
	if (c_tag) {
		collision = true;
		impactElement = source_of_impact;
	}
}


void TCompDeath::update(float delta)
 {
	//cambiar clase a manejadot de trigers
	PROFILE_FUNCTION("DeathCollider");
	if (collision) {
		if (damageTimer <= 0) {
			damageTimer = damageInterval;
			TMsgDamageToAll msg;
			msg.intensityDamage = damage;
			if (impactElement){
				impactElement->sendMsg(msg);
			}
			
		}
		else {
			damageTimer -= delta;
		}
		collision = false;
	}
}
