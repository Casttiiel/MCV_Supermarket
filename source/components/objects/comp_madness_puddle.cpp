#include "mcv_platform.h"
#include "engine.h"
#include "comp_madness_puddle.h"
#include "components/powers/comp_madness.h"


DECL_OBJ_MANAGER("comp_madness_puddle", TCompMadnessPuddle);

void TCompMadnessPuddle::debugInMenu() {
}

void TCompMadnessPuddle::load(const json& j, TEntityParseContext& ctx) {
}

void TCompMadnessPuddle::registerMsgs() {
	DECL_MSG(TCompMadnessPuddle, TMsgDamage, onPlayerAttack);
}

void TCompMadnessPuddle::onPlayerAttack(const TMsgDamage & msg) {
	dbg("Madness puddle cleansed\n");
	CEntity* p = msg.h_sender;
	TCompMadnessController* m_c = p->get<TCompMadnessController>();
	if(m_c != nullptr) {
		m_c->generateMadness(PowerType::PUDDLE);
		GameController.addMadnessPuddle();
		GameController.healPlayerPartially(10.f);
		EngineAudio.playEvent("event:/Character/Attacks/Clean_Puddle");
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
}

void TCompMadnessPuddle::update(float dt) {

}
