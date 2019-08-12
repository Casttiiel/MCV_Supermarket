#include "mcv_platform.h"
#include "engine.h"
#include "comp_madness_puddle.h"
#include "components/objects/comp_destroyable_wall.h"
#include "components/common/comp_transform.h"
#include "components/controllers/character/comp_character_controller.h"
#include "modules/module_physics.h"


using namespace physx;
DECL_OBJ_MANAGER("comp_destroyable_wall", TCompDestroyableWall);

void TCompDestroyableWall::debugInMenu() {
}

void TCompDestroyableWall::load(const json& j, TEntityParseContext& ctx) {
	typeWall = j.value("type_wall", typeWall);
}

void TCompDestroyableWall::registerMsgs() {
	DECL_MSG(TCompDestroyableWall, TMsgDamage, onPlayerAttack);
}

void TCompDestroyableWall::onPlayerAttack(const TMsgDamage & msg) {
	if (msg.damageType == FIRE && typeWall == 0) {//ice wall
		//generar animacion de descongelamiento
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
	else if (typeWall == 1) { //destroyable wall
		if (msg.damageType == MELEE)  {
			TCompTransform* c_trans = get<TCompTransform>();
			VEC3 positionWall = c_trans->getPosition();
			CHandle(this).getOwner().destroy();
			CHandle(this).destroy();
			//aparicion de prefab de cajas rotas
			TEntityParseContext ctx;
			
			ctx.root_transform.setPosition(c_trans->getPosition());
			float yaw, pitch, roll;
			ctx.root_transform.getAngles(&yaw, &pitch, &roll);

			
			ctx.root_transform.setAngles(yaw, pitch, roll);
			parseScene("data/prefabs/structures/cajasRotas.json", ctx);
			//aplicar fuerza a las a las para enviarlas hacia delante
			CHandle h_player = GameController.getPlayerHandle();
			CEntity* e_player = (CEntity*)h_player;
			TCompTransform* player_trans = e_player->get< TCompTransform>();
			physx::PxVec3 velocidad = physx::PxVec3(player_trans->getFront().x, player_trans->getFront().y, player_trans->getFront().z);
			VEC3 front_player = c_trans->getFront();
			
			//trozo arriba
			CEntity* e_trozoCajac = getEntityByName("TrozoCaja3");
			TCompCollider* c_collc = e_trozoCajac->get<TCompCollider>();
			physx::PxRigidDynamic* rigid_dynamicc = static_cast<physx::PxRigidDynamic*>(c_collc->actor);

			float impulso_yc = 3;
			float impulso_zc = 3;
			rigid_dynamicc->addForce(velocidad * impulso_zc, PxForceMode::eIMPULSE, false);
			PxVec3 velocidadyc = PxVec3(0, player_trans->getPosition().y + impulso_yc, 0);
			rigid_dynamicc->addForce(velocidadyc, PxForceMode::eIMPULSE, false);

			//trozo medio
			CEntity* e_trozoCajab = getEntityByName("TrozoCaja2");
			TCompCollider* c_collb = e_trozoCajab->get<TCompCollider>();
			physx::PxRigidDynamic* rigid_dynamicb = static_cast<physx::PxRigidDynamic*>(c_collb->actor);

			float impulso_yb = 5;
			float impulso_zb = 5;
			rigid_dynamicb->addForce(velocidad * impulso_zb, PxForceMode::eIMPULSE, false);
			PxVec3 velocidadyb = PxVec3(0, player_trans->getPosition().y + impulso_yb, 0);
			rigid_dynamicb->addForce(velocidadyb, PxForceMode::eIMPULSE, false);

			//trozo abajo
			CEntity* e_trozoCajaa = getEntityByName("TrozoCaja1");
			TCompCollider* c_colla = e_trozoCajaa->get<TCompCollider>();
			physx::PxRigidDynamic* rigid_dynamica = static_cast<physx::PxRigidDynamic*>(c_colla->actor);
			float impulso_ya = 10;
			float impulso_za = 10;
			rigid_dynamica->addForce(velocidad * impulso_za, PxForceMode::eIMPULSE, false);
			PxVec3 velocidadya = PxVec3(0, player_trans->getPosition().y + impulso_ya, 0);
			rigid_dynamica->addForce(velocidadya, PxForceMode::eIMPULSE, false);
		}
	}
	else if (typeWall == 2){//Normal Wall
		if (msg.damageType == MELEE) {

			float n = randomNumber();
			TCompTransform* c_trans = get<TCompTransform>();
			VEC3 positionWall = c_trans->getPosition();
			CHandle h_player = GameController.getPlayerHandle();
			CEntity* e_player = (CEntity*)h_player;
			TCompTransform* player_trans = e_player->get< TCompTransform>();
			physx::PxVec3 velocidad = physx::PxVec3(player_trans->getFront().x, player_trans->getFront().y, player_trans->getFront().z);
			TCompCollider* c_coll = get<TCompCollider>();
			physx::PxRigidDynamic* rigid_dynamica = static_cast<physx::PxRigidDynamic*>(c_coll->actor);
			float impulso_z = n;
			rigid_dynamica->addForce(velocidad * impulso_z, PxForceMode::eIMPULSE, false);

		}
	}


}

void TCompDestroyableWall::update(float dt) {

}

float TCompDestroyableWall::randomNumber() {
	int i;
	float num;
	srand(time(NULL));
	num = (1 + rand() % (11-1)) + 10;
	dbg("%f\n", num);
	return num;
}
