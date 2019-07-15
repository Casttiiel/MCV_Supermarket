#include "mcv_platform.h"
#include "engine.h"
#include "comp_increase_power.h"
#include "components/powers/comp_madness.h"
#include "components/controllers/character/comp_character_controller.h"
#include "ui/widgets/ui_button.h"
#include "ui/module_ui.h"
DECL_OBJ_MANAGER("comp_increase_power", TCompIncreasePower);

void TCompIncreasePower::debugInMenu() {
}

void TCompIncreasePower::load(const json& j, TEntityParseContext& ctx) {
	std::string typeName = j.value("type", typeName);


	if (strcmp("health", typeName.c_str()) == 0) {
		type = HEALTH_UP;
	}
	else if (strcmp("madness", typeName.c_str()) == 0) {
		type = MADNESS_UP;
	}
	else if (strcmp("battery", typeName.c_str()) == 0) {
		type = ACTIVATE_BATTERY;
	}
	else if (strcmp("chilli", typeName.c_str()) == 0) {
		type = ACTIVATE_CHILLI;
	}
	else if (strcmp("coffee", typeName.c_str()) == 0) {
		type = ACTIVATE_COFFEE;
	}
	else if (strcmp("teleport", typeName.c_str()) == 0) {
		type = ACTIVATE_TELEPORT;
	}
	else {
		assert(false && "PowerUp type not valid!!");
	}


	chunk = j.value("chunk", chunk);
}

void TCompIncreasePower::registerMsgs() {
	DECL_MSG(TCompIncreasePower, TMsgEntityTriggerEnter, enable);
}

void TCompIncreasePower::enable(const TMsgEntityTriggerEnter & msg) {
	if (msg.h_entity == GameController.getPlayerHandle()) {
		
		//dbg("entra en el power up \n");
		GameController.addPowerUp(chunk, type);
		destroy = true;
		if (type == ACTIVATE_BATTERY) {
			UI::CButton* b = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("card_"));
			b->setCurrentState("option_battery");
			//cambiar imagen de cursor un instante de timepo
			UI::CButton* b_cursor = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("cursor_"));
			b_cursor->setCurrentState("option_battery");
			CEntity* e_player = getEntityByName("Player");
			TCompCharacterController* c_controller = e_player->get<TCompCharacterController>();
			c_controller->power_selected = PowerType::BATTERY;


			/*UI::CButton* a = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("card_"));
			a->setCurrentState("option_teleport");
			//cambiar imagen de cursor un instante de timepo
			UI::CButton* a_cursor = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("cursor_"));
			a_cursor->setCurrentState("option_teleport");*/
		}
		if (type == ACTIVATE_TELEPORT) {
			UI::CButton* b = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("card_"));
			b->setCurrentState("option_teleport");
			//cambiar imagen de cursor un instante de timepo
			UI::CButton* b_cursor = dynamic_cast<UI::CButton*>(Engine.getUI().getWidgetByAlias("cursor_"));
			b_cursor->setCurrentState("option_teleport");
		}

		
	}
}

void TCompIncreasePower::update(float dt) {
	//una animacion o lo que sea
	if (destroy) {
		CHandle(this).getOwner().destroy();
	}
}
