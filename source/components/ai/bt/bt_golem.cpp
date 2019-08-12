#include "mcv_platform.h"
#include "engine.h"
#include "entity/entity.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/john/comp_bullet.h"
#include "components/common/comp_tags.h"
#include "time.h"
#include "stdlib.h"
#include "modules/module_physics.h"
#include "bt_golem.h"


using namespace physx;


DECL_OBJ_MANAGER("bt_golem", CBTGolem);
void CBTGolem::create(string s)//crear el arbol
{
	name = s; //createRoot y addChild debe hacerse en orden, sino peta -- nunca definir un huerfano directamente 
	// padre - hijo - tipo - condition - action
	createRoot("GOLEM", PRIORITY, NULL, NULL);

	
	addChild("GOLEM", "ON_DEATH", ACTION, (btcondition)&CBTGolem::conditionDeath, (btaction)&CBTGolem::actionDeath);
	addChild("GOLEM", "ON_IMPACT", ACTION, (btcondition)&CBTGolem::conditionImpactReceived, (btaction)&CBTGolem::actionImpactReceived);
	addChild("GOLEM", "ON_GRAVITY", ACTION, (btcondition)&CBTGolem::conditionGravityReceived, (btaction)&CBTGolem::actionGravityReceived);
	


	addChild("GOLEM", "INSIGHT", SEQUENCE, (btcondition)&CBTGolem::conditionView, NULL);//(btcondition)&CBTGolem::conditionEscape  // (btaction)&CBTGolem::actionIdle
	addChild("GOLEM", "IDLE", ACTION, NULL, (btaction)&CBTGolem::actionIdle);
	
	addChild("INSIGHT", "ALIGN", ACTION, NULL, (btaction)&CBTGolem::actionAlign);
	addChild("INSIGHT", "ATTACK", PRIORITY, NULL, NULL);

	addChild("ATTACK", "THROW", PRIORITY, (btcondition)&CBTGolem::conditionDistanceThrow, NULL);
	addChild("ATTACK", "MELEE", PRIORITY, (btcondition)&CBTGolem::conditionDistanceMelee, NULL);

	addChild("THROW", "CHARGINGTHROW", ACTION, (btcondition)&CBTGolem::conditionTimerThrow, (btaction)&CBTGolem::actionChargingThrow); //CARGANDO
	addChild("THROW", "THROWING", ACTION, NULL, (btaction)&CBTGolem::actionThrow); //LANZAR

	addChild("MELEE", "CHARGINGMELEE", ACTION, (btcondition)&CBTGolem::conditionTimerMelee, (btaction)&CBTGolem::actionChargingMelee);
	addChild("MELEE", "MELEEATACK", ACTION, NULL, (btaction)&CBTGolem::actionMelee);


	if (!h_player.isValid()) {
		h_player = getEntityByName("Player");
	}
}


int CBTGolem::actionIdle()
{
	//dbg("%s: handling idle\n", name.c_str());
	//cuando animacion este poner return stay
	return LEAVE;	
}


int CBTGolem::actionAlign()
{
	//dbg("%s: handling action align\n", name.c_str());
	
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();

	float yaw, pith;
	c_trans->getAngles(&yaw, &pith);

	float angle = rad2deg(c_trans->getDeltaYawToAimTo(player_position->getPosition()));
	if (abs(angle) <= cone_vision) {

		return LEAVE;
	}
	else {
		if (c_trans->isInLeft(player_position->getPosition())) {
			c_trans->setAngles(yaw + dt * twistSpeed, pith);
		}
		else {
			c_trans->setAngles(yaw - dt * twistSpeed, pith);
		}
		return STAY;
	}
}


int CBTGolem::actionMelee()
{
	//dbg("%s: handling action melee\n", name.c_str());
	
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();

	timerMelee = meleeFrequency;
	//crear esfera de colision
	PxSphereGeometry geometry(meleeRange);
	Vector3 damageOrigin = c_trans->getPosition() + (c_trans->getFront() * meleeRange);
	PxVec3 pos = VEC3_TO_PXVEC3(damageOrigin);
	PxQuat ori = QUAT_TO_PXQUAT(c_trans->getRotation());
	PxTransform shapePose = PxTransform(pos, ori);    // [in] initial shape pose (at distance=0)
	PxOverlapBuffer hit;
	//Evaluate collisions
	bool res = EnginePhysics.gScene->overlap(geometry, shapePose, hit, PxQueryFilterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC));
	if (hit.hasAnyHits()) {
		//Analyze entities hit
		for (int i = 0; i < hit.getNbAnyHits(); i++) {
			CHandle h_comp_physics;
			h_comp_physics.fromVoidPtr(hit.getAnyHit(i).actor->userData);
			CEntity* entityContact = h_comp_physics.getOwner();
			if (entityContact) {
				TCompTags* c_tag = entityContact->get<TCompTags>();
				if (c_tag) {
					//If the entity's tag is "player"
					std::string tag = CTagsManager::get().getTagName(c_tag->tags[0]);
					if (strcmp("player", tag.c_str()) == 0) {
						Send_DamageMessage<TMsgDamage>(entityContact, damage);
					}
				}
			}
		}
	}
	return LEAVE;
	
}

int CBTGolem::actionThrow()
{
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();

	//dbg("lanza granada");
	timerGrenade = throwFrequecy;
	TEntityParseContext ctx;
	ctx.root_transform = *c_trans;
	parseScene("data/prefabs/bullets/grenade_golem.json", ctx); //cambiar granada por algo que me invente que no explote

	TMsgAssignBulletOwner msg;
	msg.h_owner = CHandle(this).getOwner();
	msg.source = c_trans->getPosition();
	msg.front = c_trans->getFront();
	ctx.entities_loaded[0].sendMsg(msg);
	return LEAVE;

}

int CBTGolem::actionGravityReceived() {
	return LEAVE;
}

int CBTGolem::actionDeath() {
	GameController.addEnemiesKilled(EntityType::GOLEM);
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
	return LEAVE;
}

int CBTGolem::actionImpactReceived() {
	life = life - intensityDamage;
	//dbg("El golem recibe el impacto, life = %f \n", life);
	return LEAVE;
}

int CBTGolem::actionChargingThrow() {
	return LEAVE;
}

int CBTGolem::actionChargingMelee() {
	return LEAVE;
}

bool CBTGolem::conditionView()
{
	//dbg("%s: condition view\n", name.c_str());
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();
	float distancia = VEC3::Distance(c_trans->getPosition(), player_position->getPosition());
	//Esta en cono
	float angle = rad2deg(c_trans->getDeltaYawToAimTo(player_position->getPosition()));
	if (abs(angle) <= half_cone) {
		//Esta distancia 
		if (distancia <= length_cone) {
			return true;
		}

	}
	return false;
	
	
}

bool CBTGolem::conditionDistanceThrow()
{
	//dbg("%s: handling condition distance\n", name.c_str());
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();
	float distance = VEC3::Distance(player_position->getPosition(), c_trans->getPosition());
	//dbg("Distancia %f\n",distance);
	if (distance > epsilon) {
		//dbg("Dispara cupcake\n");
		return true; //---------------------------TODO PONER ESTO A TRUE
	}
	else {
		//dbg("No dispara\n");
		return false;
	}
}

bool CBTGolem::conditionDistanceMelee()
{
	//dbg("%s: handling condition distance\n", name.c_str());
	CEntity* e_player = (CEntity *)h_player;
	TCompTransform* player_position = e_player->get<TCompTransform>();
	TCompTransform* c_trans = get<TCompTransform>();
	float distance = VEC3::Distance(player_position->getPosition(), c_trans->getPosition());
	//dbg("Distancia %f\n", distance);
	if (distance < epsilon) {
		//dbg("Pega hostia\n");
		return true;
	}
	else {
		//dbg("No Pega hostia\n");
		return false;
	}
}

bool CBTGolem::conditionTimerThrow() {
	if (timerGrenade > 0) {
		timerGrenade -= dt;
		return true;

	}
	return false;
}

bool CBTGolem::conditionTimerMelee() {
	if (timerMelee > 0) {
		timerMelee -= dt;
		return true;

	}
	return false;
}

bool CBTGolem::conditionDeath() {
	return life <= 0.f;
}

bool CBTGolem::conditionImpactReceived() {

	if (life >0 && damageaded) {
		damageaded = false;
		CEntity* e_player = (CEntity *)h_player;
		TCompTransform* player_position = e_player->get<TCompTransform>();
		TCompTransform* c_trans = get<TCompTransform>();

		float yaw, pith;
		c_trans->getAngles(&yaw, &pith);

		float angle = rad2deg(c_trans->getDeltaYawToAimTo(player_position->getPosition()));
		if ( abs(angle) > cone_vision) {
			return true;

		}
	}

	return false;
}


bool CBTGolem::conditionGravityReceived() {
	if (battery_time > 0) {
		battery_time -= dt;
		//dbg("battery time = %f \n", battery_time);
		return true;
	
	}
	return false;
}



template <typename T>
void CBTGolem::Send_DamageMessage(CEntity* entity, float dmg) {
	//Send damage mesage
	T msg;
	// Who sent this bullet
	msg.h_sender = h_sender;
	// The bullet information 
	msg.h_bullet = CHandle(this).getOwner();
	msg.intensityDamage = dmg;
	msg.senderType = ENEMIES;
	msg.targetType = PLAYER;
	msg.impactForce = impactForceAttack;
	entity->sendMsg(msg);

	//dbg("ataque melee enviado al player \n");
}

void CBTGolem::load(const json& j, TEntityParseContext& ctx) {
	

	half_cone = j.value("halfCone", half_cone);
	length_cone = j.value("lengthCone", length_cone);
	hearing_radius = j.value("hearingRadius", hearing_radius);
	life = j.value("life", life);
	damageArea = j.value("damageArea", damageArea);

	this->create("Golem_bt");

}


void CBTGolem::debugInMenu() {
	
}

void CBTGolem::renderDebug() {
	TCompTransform* c_trans = get<TCompTransform>();
	TCompRender* c_render = get<TCompRender>();
	VEC3 front = c_trans->getFront();
	VEC3 pos = c_trans->getPosition();
	float angle = deg2rad(half_cone);

	//Create a rotation matrix with the angle
	MAT44 aux_cone_1 = MAT44::CreateRotationY(angle);
	MAT44 aux_cone_2 = MAT44::CreateRotationY(-angle);

	//Create two vectors to store the result
	VEC3 half_cone_1, half_cone_2;

	//We rotate the vector "front" with the matrix "aux_cone_X" into "half_cone_X"
	VEC3::Transform(front, aux_cone_1, half_cone_1);
	VEC3::Transform(front, aux_cone_2, half_cone_2);
	half_cone_1.Normalize();
	half_cone_2.Normalize();

	drawCircle(pos, hearing_radius, c_render->color);
	drawLine(pos, pos + half_cone_1 * length_cone, c_render->color);
	drawLine(pos, pos + half_cone_2 * length_cone, c_render->color);
	drawLine(pos + half_cone_1 * length_cone, pos + half_cone_2 * length_cone, c_render->color);
}



void CBTGolem::onCollision(const TMsgOnContact& msg) {
	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();

	TCompTags* c_tag = source_of_impact->get<TCompTags>();
	if (c_tag) {
		std::string tag = CTagsManager::get().getTagName(c_tag->tags[0]);
		if (strcmp("floor", tag.c_str()) == 0) {
			return;
		}
		else if (strcmp("enemy", tag.c_str()) == 0) {
			return;
		}
		else if (strcmp("player", tag.c_str()) == 0) {
			return;
		}
		else if (strcmp("grenade", tag.c_str()) == 0) {
			//ChangeState("IMPACT");
			//dbg("Daño granada");
			
		}
		else if (strcmp("bullet", tag.c_str()) == 0) {
			//ChangeState("IMPACT");
			//dbg("Daño bala");
		}
	}
}


void CBTGolem::onGravity(const TMsgGravity& msg) {
	//	Paralyze golem during a timer

	h_sender = msg.h_sender;
	h_bullet = msg.h_bullet;
	//obtener posicion

	CEntity* e_bullet = (CEntity *)h_bullet;
	TCompTransform* bullet_trans = e_bullet->get<TCompTransform>();
	//battery_position = bullet_trans->getPosition();
	battery_time = msg.time_effect;
}
/*
void CAIGolem::BatteryEfectState(float dt) {

	if (battery_time > 0) {
		battery_time -= dt;
		//NOTHING

		dbg("battery time = %f \n", battery_time);
	}
	else {
		dbg("entra en align");
		ChangeState("ALIGN");
	}

}
*/

void CBTGolem::onDamageInfoMsg(const TMsgDamage& msg) {
	/*h_sender = msg.h_sender;
	CEntity* e_source = (CEntity *)msg.h_bullet;
	TCompTransform* trans_source = e_source->get<TCompTransform>();
	life -= msg.intensityDamage;
	if (life < 0)
		life = 0;*/
	h_sender = msg.h_sender;
	CEntity* e_source = (CEntity *)msg.h_bullet;
	TCompTransform* trans_source = e_source->get<TCompTransform>();
	intensityDamage = msg.intensityDamage;
	damageaded = true;
}


void CBTGolem::registerMsgs() {
	DECL_MSG(CBTGolem, TMsgDamage, onDamageInfoMsg);
	DECL_MSG(CBTGolem, TMsgOnContact, onCollision);
	DECL_MSG(CBTGolem, TMsgGravity, onGravity);
	DECL_MSG(CBTGolem, TMsgBTPaused, onMsgBTPaused);
}






