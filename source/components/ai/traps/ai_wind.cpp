#include "mcv_platform.h"
#include "ai_wind.h"
#include "engine.h"
#include "entity/entity.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/common/comp_tags.h"
#include "time.h"
#include "stdlib.h"
#include "modules/module_physics.h"

using namespace physx;


DECL_OBJ_MANAGER("comp_wind", TCompWind);


void TCompWind::update(float dt)
{
	TCompTransform* c_trans = get<TCompTransform>();


	if (firstTime) {
		TEntityParseContext ctx;
		ctx.root_transform = *c_trans;
		parseScene("data/prefabs/traps/wind.json", ctx);
		TCompCollider* c_col = get<TCompCollider>();
		physx::PxRigidDynamic* rigid_dynamic = static_cast<physx::PxRigidDynamic*>(c_col->actor);

		PxBoxGeometry box;
		PxShape* colShape;
		rigid_dynamic->getShapes(&colShape, 1, 0);
		colShape->getBoxGeometry(box);
		VEC3 creationWindOffset = VEC3(c_trans->getPosition().x, (c_trans->getPosition().y + (box.halfExtents.y)), c_trans->getPosition().z + (box.halfExtents.z));


		TMsgAssignBulletOwner msg;
		msg.h_owner = CHandle(this).getOwner();
		msg.source = creationWindOffset;
		msg.front = c_trans->getFront();
		ctx.entities_loaded[0].sendMsg(msg);
		firstTime = false;
	}
}

void TCompWind::debugInMenu() {

	
}

void TCompWind::load(const json& j, TEntityParseContext& ctx) {
	_durationOn = j.value("_durationOn", _durationOn);
	_durationOn_init = j.value("_durationOn_init", _durationOn_init);
	_durationOff = j.value("_durationOff", _durationOff);
	_durationOff_init = j.value("_durationOff_init", _durationOff_init);
	_windDamage = j.value("_windDamage", _windDamage);
}

void TCompWind::onBattery(const TMsgGravity & msg) {
	//Animate or start particle system, do something
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
}



void  TCompWind::registerMsgs() {
	DECL_MSG(TCompWind, TMsgGravity, onBattery);
}
/*void TCompWind::onBattery(const TMsgGravity & msg) {
	
	//Animate or start particle system, do something
	CHandle(this).getOwner().destroy();
	CHandle(this).destroy();
}
*/

