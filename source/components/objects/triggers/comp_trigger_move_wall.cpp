#include "mcv_platform.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_name.h"
#include "engine.h"
#include "comp_trigger_move_wall.h"
#include "components/common/comp_id.h"



DECL_OBJ_MANAGER("trigger_move_wall", TCompTriggerMoveWall);

void TCompTriggerMoveWall::debugInMenu() {
}

void TCompTriggerMoveWall::load(const json& j, TEntityParseContext& ctx) {

	if (j.count("idElementsToMove")) {
		std::vector< int > ids = j["idElementsToMove"].get< std::vector< int >>();
		
		for (int i = 0; i <ids.size(); i++) {
			//dbg("pilla los elementos to move %i \n", ids[i]);
			CEntity* e_wall = getEntityById(ids[i]);
			if (e_wall != nullptr) {
				//dbg("pilla los elementos to move y son validos %i \n", ids[i]);
				elementsToMove.push_back(e_wall);
			}
		}
	}
	

	/* Dudas existenciales: 
	TODO:
		//probar con todos los tipos de plataforma
	*/


}

void TCompTriggerMoveWall::registerMsgs()
{
	DECL_MSG(TCompTriggerMoveWall, TMsgEntityTriggerEnter, onEnter);
	DECL_MSG(TCompTriggerMoveWall, TMsgEntityTriggerExit, onExit);
	//DECL_MSG(TCompTriggerMoveWall, TMsgSetActive, onActiveMsg);
}

void TCompTriggerMoveWall::onEnter(const TMsgEntityTriggerEnter & msg)
{
	//dbg("el player entra en el trigger de move wall \n");
	CHandle h_player = EngineEntities.getPlayerHandle();
	if (h_player == msg.h_entity) {
		if (!elementsToMove.empty()) {
			for (int i = 0; i < elementsToMove.size(); i++) {
				TMsgSetActive msgToSend;
				TCompID *c_id = get<TCompID>();
				msgToSend.active = false;
				elementsToMove[i]->sendMsg(msgToSend);
			}
		}
	}

	destroy = true;
}

void TCompTriggerMoveWall::onExit(const TMsgEntityTriggerExit & msg) {
/* //Descomentar esto si queremos que las cosas paren cuando sales del trigger
CHandle h_player = EngineEntities.getPlayerHandle();
if (h_player == msg.h_entity) {
if (!elementsToMove.empty()) {
for (int i = 0; i < elementsToMove.size(); i++) {
TMsgSetActive msgToSend;
msgToSend.active = true;
elementsToMove[i]->sendMsg(msgToSend);
}
}
}
*/

}

void TCompTriggerMoveWall::update(float dt)
{
	if (destroy) {
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}

}
