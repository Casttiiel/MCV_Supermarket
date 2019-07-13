#include "mcv_platform.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "engine.h"
#include "comp_remote_trigger.h"
#include "components/common/comp_id.h"



DECL_OBJ_MANAGER("comp_remote_trigger", TCompRemoteTrigger);

void TCompRemoteTrigger::debugInMenu() {
}

void TCompRemoteTrigger::load(const json& j, TEntityParseContext& ctx) {

	if (j.count("entities_actions")) {
		
		auto& ent_act = j["entities_actions"];
		assert(ent_act.is_array());
		for (int i = 0; i < ent_act.size(); ++i) {
			CEntity* ent = getEntityById(ent_act[i]["id"]);
			assert(ent);
			entities.push_back(ent);
			actions.push_back(ent_act[i]["activate"]);
		}
	}

	if (entities.size() != actions.size()) {
		fatal("Remote trigger not properly defined\n");
	}
}

void TCompRemoteTrigger::registerMsgs()
{
	DECL_MSG(TCompRemoteTrigger, TMsgEntityTriggerEnter, onEnter);
}

void TCompRemoteTrigger::onEnter(const TMsgEntityTriggerEnter & msg)
{
	CHandle h_player = EngineEntities.getPlayerHandle();
	if (h_player == msg.h_entity) {
		if (!entities.empty() && !actions.empty()) {
			for (int i = 0; i < entities.size(); i++) {
				TMsgSetActive msgToSend;
				msgToSend.active = actions[i];
				entities[i]->sendMsg(msgToSend);
			}
		}
	}
}

void TCompRemoteTrigger::update(float dt)
{

}
