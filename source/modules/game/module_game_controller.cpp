#include "mcv_platform.h"
#include "engine.h"
#include "module_game_controller.h"
#include "input/devices/device_mouse.h"
#include "components/common/comp_tags.h"
#include "components/controllers/character/comp_character_controller.h"
#include "entity/common_msgs.h"
#include "entity/msgs.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "windows/app.h"
#include "components/objects/checkpoints/checkpoint.h"
#include "components/common/comp_tags.h"
#include "input/module_input.h"
#include "components/common/physics/comp_rigid_body.h"
#include "components/common/comp_transform.h"
#include "components/ai/bt/bt_sushi.h"
#include "modules/gameplay_fragments/module_ambush.h"
#include "components/ai/bt/bt_ranged_sushi.h"
#include "components/controllers/comp_curve_controller.h"
#include "components/controllers/comp_trans_curve_controller.h"
#include "modules/module_camera_mixer.h"
#include "geometry/interpolators.h"
#include "components/controllers/camera/comp_camera_3rd_person.h"
#include "components/ai/graph/ai_platform.h"
#include "components/common/comp_name.h"
#include "entity/entity.h"
#include "modules/module_scenes.h"


bool CModuleGameController::start() {

   /* window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_width = 250;
    window_height = 150;

    _player = EngineEntities.getPlayerHandle();

    _lastCheckpoint = new CCheckpoint();
    _currentstate = PauseState::none;
    menuPosition = 0;
    */
    _lastCheckpoint = new CCheckpoint();
    return true;
}

void CModuleGameController::setGodMode(bool _god_mode) {
	god_mode = _god_mode; 
}

void CModuleGameController::update(float delta) {

    //updateGameCondition();
  if (EngineInput["godMode_"].justPressed()) {
    god_mode = !god_mode;
  }
  else if (EngineInput["flyMode_"].justPressed()) {
    CEntity* e_player = getEntityByName("Player");
  }
  else if (EngineInput["block_invisible_cursor_"].justPressed()) {
	  invisible_block_cursor = !invisible_block_cursor;
  }

}

void CModuleGameController::stop()
{
    if (_lastCheckpoint) {
        delete _lastCheckpoint;
		_lastCheckpoint = nullptr;
    }
}

CHandle CModuleGameController::getPlayerHandle() {
	return EngineEntities.getPlayerHandle();
}

#pragma region Checkpoints
bool CModuleGameController::isCheckpointSaved() {
	return _lastCheckpoint != nullptr && _lastCheckpoint->isSaved(); 
}
/*

This function calls CCheckpoint::saveCheckPoint with the player position and rotation
This updates _lastCheckpoint's data with the status of those entities marked to be registered

*/
bool CModuleGameController::saveCheckpoint(VEC3 playerPos, QUAT playerRot) {

    return _lastCheckpoint->saveCheckPoint(playerPos, playerRot);
}

bool CModuleGameController::loadCheckpoint() {

    if (_lastCheckpoint) {
        return _lastCheckpoint->loadCheckPoint();
    }
    else {
        return false;
    }
}

bool CModuleGameController::deleteCheckpoint() {

    if (_lastCheckpoint) {
        return _lastCheckpoint->deleteCheckPoint();
    }
    else {
        return false;
    }
}
#pragma endregion

CModuleGameController::PauseState CModuleGameController::getCurrentState() {

    return _currentstate;
}


#pragma region Behavior Tree Control
void CModuleGameController::stopStateMachines() {
    VHandles handles_in_scene = CTagsManager::get().getAllEntitiesByTag(getID("enemy"));
    for (auto& instance : handles_in_scene) {
        TMsgAIPaused msgPaused;
        msgPaused.isPaused = true;
        instance.sendMsg(msgPaused);
    }
}

void CModuleGameController::stopBehaviorTrees() {
	VHandles handles_in_scene = CTagsManager::get().getAllEntitiesByTag(getID("enemy"));
	for (auto& instance : handles_in_scene) {
		TMsgBTPaused msgPaused;
		msgPaused.isPaused = true;
		instance.sendMsg(msgPaused);
	}	
}

void CModuleGameController::resumeBehaviorTrees() {
	VHandles handles_in_scene = CTagsManager::get().getAllEntitiesByTag(getID("enemy"));
	for (auto& instance : handles_in_scene) {
		TMsgBTPaused msgPaused;
		msgPaused.isPaused = false;
		instance.sendMsg(msgPaused);
	}
}
#pragma endregion

#pragma region Entity Handling
//Spawn a prefab and get it's CHandle
CHandle CModuleGameController::spawnPrefab(const std::string prefab, Vector3 pos, QUAT rot, float scale) {
	TEntityParseContext ctx;
  ctx.root_transform.setPosition(pos);
  ctx.root_transform.setRotation(rot);
  ctx.root_transform.setScale(scale);
	bool result = parseScene(prefab, ctx);
	assert(result);
	return ctx.entities_loaded[0];
}
//
void CModuleGameController::toNamePrefab(const std::string name, CHandle handle) {
	CEntity* e = handle;
	TCompName* c_name = e->get<TCompName>();
}



//lo que se quiere es spawnear algo en la posicion de otra entidad dado el nombre
//ej:spawnSushy("Dummy1")
void CModuleGameController::spawnPrefabByPoint(const std::string prefab, std::string origin) {
	CEntity* entity = (CEntity *) getEntityByName(origin);
	TCompTransform* transform = entity->get<TCompTransform>();
	spawnPrefab(prefab, transform->getPosition());
}


//danyo a una entidad por el nombre
void CModuleGameController::setDamage(const std::string name_origin, int damage) {
	CEntity* entity = (CEntity *)getEntityByName(name_origin);
	TMsgDamageToAll message;
	message.intensityDamage = damage;
	entity->sendMsg(message);
}

//bindear componente curva
void CModuleGameController::bindInCurve(std::string _curve, std::string name_origin) {

	CHandle  h_entity = getEntityByName(name_origin);
	CEntity* entity = h_entity;
	CEntity* entity_curve = getEntityByName(_curve);
	CHandle h_curve  = entity_curve->get<TCompTransCurveController>();
	entity->set(h_curve.getType(), h_curve);
	
	dbg("assignando handle %08x to emtity %08x\n", h_curve.asUnsigned(), h_entity.asUnsigned());
	dbg("Componente padre %08x \n", h_curve.getOwner());
	
	//CHandle enhandletity = getEntityByName(name_origin);
	//enhandletity.setOwner(tcompCurve);
	//entity->set(handle.getType(), handle);
}
//blendear camera
void CModuleGameController::blendingCamera(std::string camDest, float blendTime, std::string typeInterpolation) {
	CHandle  h_entity = getEntityByName(camDest);
	if(h_entity.isValid()){
		Engine.getCameraMixer().blendCamera(h_entity, blendTime, Engine.getCameraMixer().getInterpolator(typeInterpolation));
	}
}

//bloquearCamera Player
void CModuleGameController::lockCamera3Person(bool activate) {
	CEntity* e_cam3 = getEntityByName("PlayerCamera");
	TCompCamera3rdPerson* t_comp3 = e_cam3->get<TCompCamera3rdPerson>();
	if (t_comp3 != nullptr) {
		//t_comp3->_enabled = activate;
		t_comp3->mouse_active = activate;
	}
}

//resetCamera
void CModuleGameController::resetCamera() {
	CEntity* e_cam3 = getEntityByName("PlayerCamera");
	TCompCamera3rdPerson* t_comp3 = e_cam3->get<TCompCamera3rdPerson>();
	if (t_comp3 != nullptr) {
		//t_comp3->_enabled = activate;
		t_comp3->resetCamera();
	}

}



void CModuleGameController::destroyCHandleByName(std::string name) {
	if (name != "") {
		CHandle h = getEntityByName(name);
		if (h.isValid()) {
			h.destroy();
		}
	}
}

void CModuleGameController::activatePlatformByName(std::string name) {
	if (name != "") {
		CEntity* e_platform = getEntityByName(name);
		CAIMobilePlatform* c_platform = e_platform->get<CAIMobilePlatform>()  ;
		c_platform->active = true;
	}
}

//Spawn a puddle and get it's CHandle
CHandle CModuleGameController::spawnPuddle(Vector3 pos, QUAT rot, float scale) {
    VEC3 desiredPosition = pos;
    desiredPosition.y += 0.001f; //So we can detect collision with planes
    physx::PxRaycastHit hit;
    physx::PxFilterData pxFilterData;
    pxFilterData.word0 = EnginePhysics.Scenario;
    physx::PxQueryFilterData PxScenarioFilterData;
    PxScenarioFilterData.data = pxFilterData;
    PxScenarioFilterData.flags = physx::PxQueryFlag::eSTATIC;
    EnginePhysics.Raycast(desiredPosition, VEC3(0, -1, 0), 30.f, hit, physx::PxQueryFlag::eSTATIC, PxScenarioFilterData);
    if (hit.position != physx::PxVec3(0)) {
        desiredPosition = PXVEC3_TO_VEC3(hit.position);
    }

    return spawnPrefab("data/prefabs/structures/madness_puddle.json", desiredPosition, rot, scale);;
}

void CModuleGameController::deleteByTag(const char* tag) {
	VHandles handles_in_scene = CTagsManager::get().getAllEntitiesByTag(getID(tag));
	for (auto& instance : handles_in_scene) {
		auto hm = CHandleManager::getByType(instance.getType());
		if (hm)
			hm->destroyHandle(instance);
		instance.getOwner().destroy();
		instance.destroy();
	}
}
#pragma endregion

#pragma region Time Control
void CModuleGameController::stopTime() {
	setTimeScale(0.0f);
}

void CModuleGameController::resumeTime() {
	setTimeScale(1.0f);
}

void CModuleGameController::setTimeScale(float timeScale){
	assert(timeScale >= 0.0f);
	Time.scale_factor = timeScale;
}

float CModuleGameController::getTimeScale(){
	return Time.scale_factor;
}
#pragma endregion

void CModuleGameController::addMadnessPuddle() {
	_madnessPuddlesCleansed++;
}

int CModuleGameController::getMadnessPuddles() {
	return _madnessPuddlesCleansed;
}

void CModuleGameController::addEnemiesKilled(EntityType e) {
  if (gf)
    gf->enemyKilled(e);
	enemiesKilled++;
}

int CModuleGameController::getEnemiesKilled() {
	return enemiesKilled;
}

float CModuleGameController::getHpBarSize() {
	return hp_bar_size;
}

void CModuleGameController::increaseHpBarSize(float _hp_bar_size) {
	hp_bar_size += _hp_bar_size;
}

void CModuleGameController::increaseMadnessBarSize(float _madness_bar_size) {
	madness_bar_size += _madness_bar_size;
}

float CModuleGameController::getMadnessBarSize() {
	return madness_bar_size;
}




void CModuleGameController::generateDamageSphere(VEC3 center, float radius, TMsgDamage message, const char* targetFilter) {
	//Create a geometry
	physx::PxSphereGeometry geometry(radius);
	//Create filter data
	physx::PxFilterData pxFilterData;
	pxFilterData.word0 = EnginePhysics.getFilterByName(targetFilter);
	//Create query filter data
	physx::PxQueryFilterData queryFilterData;
	queryFilterData.data = pxFilterData;
	queryFilterData.flags = physx::PxQueryFlag::eDYNAMIC;
	//Create vector to store the hits
	std::vector<physx::PxOverlapHit> hits;
	//Overlap and check for collissions
	if (EnginePhysics.Overlap(geometry, center, hits, queryFilterData)) {
		for (int i = 0; i < hits.size(); i++) {
			CHandle hitCollider;
			hitCollider.fromVoidPtr(hits[i].actor->userData);
			if (hitCollider.isValid()) {
				//Send the message we've been given
				CEntity* entityContact = hitCollider.getOwner();
				entityContact->sendMsg(message);
				//Check if we can apply force to the entity
				/*TCompRigidBody* c_rb = entityContact->get<TCompRigidBody>();
				if (c_rb && impactForce != 0.f) {
					TCompTransform* entityTransform = entityContact->get<TCompTransform>();
					VEC3 resultingForce = entityTransform->getPosition() - center;
					resultingForce.Normalize();
					resultingForce *= impactForce;
					c_rb->addForce(resultingForce);
				}*/
			}
		}
	}
}

void CModuleGameController::healPlayer() {
	CEntity* e_player = (CEntity *)EngineEntities.getPlayerHandle();
	TCompCharacterController* character_controller = e_player->get<TCompCharacterController>();
	character_controller->heal();
}

void CModuleGameController::healPlayerPartially(float health) {
    CEntity* e_player = (CEntity *)EngineEntities.getPlayerHandle();
    TCompCharacterController* character_controller = e_player->get<TCompCharacterController>();
    character_controller->healPartially(health);
}

void CModuleGameController::restoreMadness() {
	CEntity* e_player = (CEntity *)EngineEntities.getPlayerHandle();
	TCompCharacterController* character_controller = e_player->get<TCompCharacterController>();
	character_controller->restoreMadness();
}

void CModuleGameController::addPowerUp(float quantity, PowerUpType type) {

	//de momento el isLife seria el tipo y solo diferenciamos entre si es life o es energia/locura
	//en un futuro lo cambiamos por un type para usarlo en los demas powerUps
	//dbg("el game controller envia el power up de life  \n");
	CEntity* e_player = (CEntity *) EngineEntities.getPlayerHandle();
	TCompCharacterController* character_controller = e_player->get<TCompCharacterController>();
	character_controller->applyPowerUp(quantity,type, extraBarSize);

}

void CModuleGameController::renderInMenu() {
  if (ImGui::TreeNode("Game Controller"))
  {
    if (gf)
      gf->renderInMenu();
    ImGui::TreePop();
  }
}

void CModuleGameController::activateGameplayFragment(string module) {
  //fatal(module.c_str());
  if (gf) { //JUST IN CASE
    gf->stop();
  }

  gf = new CModuleAmbush(1); //SHIT HERE
  gf->start();
}



void CModuleGameController::deactivateGameplayFragment(string module) {
  //fatal(module.c_str());
  if (gf) {
    gf->stop();
  }
}

void CModuleGameController::activateSecondEvent() {
	//fatal(module.c_str());
	if (gf) {
		gf->secondEvent();
	}
}

int CModuleGameController::getNumThermoStatesActives() {
	if(gf != nullptr){
		return gf->getThermostatsActivates();
	}
	return 0;
}

void CModuleGameController::wakeUpSushi(string id) {
	
	CHandle sushi = getEntityByName("Sushi_"+id);
	if (sushi.isValid()) {
		TMsgBTPaused msgPaused;
		msgPaused.isPaused = false;
		sushi.sendMsg(msgPaused);
	}
}

void CModuleGameController::deleteElement(string id) {

	CHandle element = getEntityByName(id);

	if (element.isValid()) {
		element.destroy();
	}

}

void CModuleGameController::activateSpawner(string id) {

	CHandle spawner = getEntityByName(id);
	if (spawner.isValid()) {
		TMsgSetActive msgPaused;
		msgPaused.active = true;
		spawner.sendMsg(msgPaused);
	}
}

/*
_curve: nombre de la curva

enemy = nombre de la entidad

*/
void CModuleGameController::updateEnemyCurveByName(std::string _curve, std::string enemy) {

	CHandle h_enemy = getEntityByName(enemy);
	if (h_enemy.isValid()) {
		std::string path = "data/curves/" + _curve + ".curve";
		//TENER EN CUENTA QUE SI ES UN RANGED O NO
		CEntity* e_enemy = (CEntity*)h_enemy;


		CBTSushi* sushi = e_enemy->get<CBTSushi>();
		if (sushi != nullptr) {
			sushi->setCurve(Resources.get(path)->as<CCurve>());
		}
		else {
			CBTRangedSushi* sushi = e_enemy->get<CBTRangedSushi>();
			sushi->setCurve(Resources.get(path)->as<CCurve>());
		}
	}
}

void  CModuleGameController::updateEnemyCurveByHandle(std::string _curve, CHandle h_enemy) {
	if (h_enemy.isValid()) {
		std::string path = "data/curves/" + _curve + ".curve";
		//TENER EN CUENTA QUE SI ES UN RANGED O NO
		CEntity* e_enemy = (CEntity*)h_enemy;


		CBTSushi* sushi = e_enemy->get<CBTSushi>();
		if (sushi != nullptr) {
			sushi->setCurve(Resources.get(path)->as<CCurve>());
		}
		else {
			CBTRangedSushi* sushi = e_enemy->get<CBTRangedSushi>();
			sushi->setCurve(Resources.get(path)->as<CCurve>());
		}
	}
}


void CModuleGameController::setPauseEnemyByName(std::string enemy, bool active) {
	
	CEntity* e_enemy = getEntityByName(enemy);
	if (e_enemy != nullptr) {
		TMsgBTPaused msg;
		msg.isPaused = active;
		e_enemy->sendMsg(msg);
	}
}

void CModuleGameController::setPauseEnemyByHandle(CHandle h_enemy, bool active) {

	CEntity* e_enemy = h_enemy;
	if (e_enemy != nullptr) {
		TMsgBTPaused msg;
		msg.isPaused = active;
		e_enemy->sendMsg(msg);
	}
}

void CModuleGameController::setTransformObject(std::string name,VEC3 pos,float yaw, float pith, float roll) {
	if (name != "") {
		CHandle handle = getEntityByName(name);
		CEntity* entity = handle;
		TCompTransform* transform = entity->get<TCompTransform>();
		transform->setPosition(pos);
		transform->setAngles(deg2rad(yaw), deg2rad(pith), deg2rad(roll));

		QUAT quat = transform->getRotation();
		//VEC3_TO_PXVEC3()
		TCompCollider* collider = entity->get<TCompCollider>();
		//collider->setGlobalPose(QUAT_TO_PXQUAT(quat););
		collider->setGlobalPose(pos,quat);

		//transform->setScale(scale);
	} 
}


void CModuleGameController::inCinematic(bool active) {
	CHandle e_player = getEntityByName("Player");
	TMsgOnCinematic msgOnCinematic;
	msgOnCinematic.cinematic = active;
	e_player.sendMsg(msgOnCinematic);
		
	
}

void CModuleGameController::loadScene(const std::string name) {
	SceneManager.getSceneManager()->loadScene(name);
}



template <typename T>
void CModuleGameController::broadcastMessage(T message) {
	//EngineEntities.
}

//function casts

TCompCurveController* TCurveController(CHandle h)
{
	TCompCurveController* t = h;
	return t;
}

CEntity* toEntity(CHandle h)
{
	CEntity* e = h;
	return e;
}
TCompSkelLookAt* toCompSkelLookAt(CHandle h)
{
	TCompSkelLookAt* t = h;
	return t;
}


