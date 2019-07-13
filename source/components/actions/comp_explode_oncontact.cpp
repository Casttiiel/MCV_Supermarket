#include "mcv_platform.h"
#include "comp_explode_oncontact.h"
#include "components/common/comp_transform.h"
#include "engine.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_explode_oncontact", TCompExplodeOnContact);

void TCompExplodeOnContact::debugInMenu() {
}

void TCompExplodeOnContact::load(const json& j, TEntityParseContext& ctx) {
	_explosionForce = j.value("_explosionForce", _explosionForce);
	_explosionDamage = j.value("_explosionDamage", _explosionDamage);
	_explosionRadius = j.value("_explosionRadius", _explosionRadius);
}

void TCompExplodeOnContact::registerMsgs() {
  DECL_MSG(TCompExplodeOnContact, TMsgOnContact, onCollision);
}

void TCompExplodeOnContact::onCollision(const TMsgOnContact& msgC) {
	TCompTransform* c_trans = get<TCompTransform>();
	TMsgDamage msg;
	msg.senderType = EntityType::ENVIRONMENT;
	msg.targetType = EntityType::ALL;
	msg.position = c_trans->getPosition();
	msg.intensityDamage = _explosionDamage;
	msg.impactForce = _explosionForce;
	GameController.generateDamageSphere(c_trans->getPosition(), _explosionRadius, msg, "VulnerableToExplosions");
	_collided = true;
	GameController.spawnPrefab("data/prefabs/props/explosion_sphere.json", c_trans->getPosition());
}

void TCompExplodeOnContact::update(float dt) {
	if (_collided) {
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
}