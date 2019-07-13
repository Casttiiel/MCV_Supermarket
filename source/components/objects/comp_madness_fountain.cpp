#include "mcv_platform.h"
#include "engine.h"
#include "comp_madness_fountain.h"
#include "components/powers/comp_madness.h"


DECL_OBJ_MANAGER("comp_madness_fountain", TCompMadnessFountain);

void TCompMadnessFountain::debugInMenu() {
}

void TCompMadnessFountain::load(const json& j, TEntityParseContext& ctx) {
}

void TCompMadnessFountain::registerMsgs() {
	DECL_MSG(TCompMadnessFountain, TMsgEntityTriggerEnter, enable);
	DECL_MSG(TCompMadnessFountain, TMsgEntityTriggerExit, disable);
}

void TCompMadnessFountain::enable(const TMsgEntityTriggerEnter & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = true;
	}
}

void TCompMadnessFountain::disable(const TMsgEntityTriggerExit & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		_isEnabled = false;
	}
}

void TCompMadnessFountain::update(float dt) {
	if (_isEnabled) {
		CEntity* p = GameController.getPlayerHandle();
		TCompMadnessController* m_c = p->get<TCompMadnessController>();
    float value = m_c->getPowerGeneration(PowerType::FOUNTAIN) * dt;
		m_c->generateMadness(value);
    GameController.healPlayerPartially(value);
	}
}
