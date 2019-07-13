#include "mcv_platform.h"
#include "power_up.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_tags.h"
#include "components/common/comp_render.h"
#include "entity/entity_parser.h"
#include "engine.h"
#include "input/input.h"
#include "input/module_input.h"
#include "modules/module_physics.h"

using namespace physx;

DECL_OBJ_MANAGER("power_up", TCompPowerUp);


void TCompPowerUp::load(const json& j, TEntityParseContext& ctx) {
	power_type = j.value("power_type", power_type);
}

void TCompPowerUp::registerMsgs() {
	DECL_MSG(TCompPowerUp, TMsgOnContact, onCollision);
}

void TCompPowerUp::onCollision(const TMsgOnContact& msg) {

	CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
	TCompTags* c_tag = source_of_impact->get<TCompTags>();
	if (c_tag) {
		std::string tag = CTagsManager::get().getTagName(c_tag->tags[0]);
		if (strcmp("player", tag.c_str()) == 0) {
			destroy = true;
			dbg("envia powerUp al player \n");
			TMsgPowerUp msg;
			msg.power_type = power_type;
			source_of_impact->sendMsg(msg);
			//molaria abrir una ventana de power up TODO
		}
	}
}


void TCompPowerUp::update(float delta)
 {
	PROFILE_FUNCTION("PowerUp");
	if (destroy) {
		CHandle(this).getOwner().destroy();
	}
}
