#include "mcv_platform.h"
#include "engine.h"
#include "components/common/comp_transform.h"
#include "components/objects/comp_interruptor.h"


DECL_OBJ_MANAGER("comp_interruptor", TCompInterruptor);

void TCompInterruptor::debugInMenu() {



}

void TCompInterruptor::load(const json& j, TEntityParseContext& ctx) {
	nombre = j.value("nombre", nombre);
	elementoAmover = j.value("elementoAmover", elementoAmover);
}

void TCompInterruptor::registerMsgs() {
	DECL_MSG(TCompInterruptor, TMsgDamage, onActivate);
}

void TCompInterruptor::onActivate(const TMsgDamage & msg) {
	if (msg.damageType == MELEE) {
		//llamar a script activateAlgo y nombre de lo que se activa
		if(!isOn){
			//cambiar por nombres que nos pasen en el mapa
			if (nombre.compare("panelRampa") == 0) {
				
			 	Scripting.execActionDelayed("activePlatformByName(\"Box040\")", 0.0);
			    GameController.resetCamera();
				execDelayedAction("on_cinematic(true)", 0.1);
				execDelayedAction("on_lock_camera3(false)", 0.2);
				execDelayedAction("on_blending_camera(\"CameraRampa002\", 4,\"linear\")", 0.5);
				execDelayedAction("on_blending_camera(\"PlayerCamera\", 4,\"linear\")", 6);
				execDelayedAction("on_lock_camera3(true)", 9);
				execDelayedAction("on_cinematic(false)", 9);


			}
			/*else if (nombre.compare("panel_prueba2") == 0) {
				//nombre de plataforma a mover
				Scripting.execActionDelayed("activePlatformByName(\"zanahoria2\")", 0.0);
				//nombre de camaras para mostrar lo que vamos ha hacer
			}*/

			isOn = true;
		}
		CHandle(this).getOwner().destroy();
		CHandle(this).destroy();
	}
}
	

