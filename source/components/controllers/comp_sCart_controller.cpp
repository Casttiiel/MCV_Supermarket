#include "mcv_platform.h"
#include "components/controllers/character/comp_character_controller.h"
#include "components/common/physics/comp_rigid_body.h"
#include "comp_sCart_controller.h"
#include "components/common/physics/comp_collider.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_tags.h"
#include "components/common/comp_camera.h"
#include "components/common/comp_render.h"
#include "modules/module_physics.h"
#include "engine.h"
#include "input/module_input.h"
#include "ui/widgets/ui_image.h"
#include "ui/module_ui.h"
#include "ui/ui_widget.h"

using namespace physx;

DECL_OBJ_MANAGER("sCart_controller", TCompSCartController);

void TCompSCartController::Init() {
	AddState("SCART_DISABLED", (statehandler)&TCompSCartController::disabled);
	AddState("SCART_GROUNDED", (statehandler)&TCompSCartController::grounded);
	AddState("SCART_ROWING", (statehandler)&TCompSCartController::rowing);
	AddState("SCART_ON_AIR", (statehandler)&TCompSCartController::onAir);
	AddState("SCART_DAMAGED", (statehandler)&TCompSCartController::damaged);
	AddState("SCART_DEAD", (statehandler)&TCompSCartController::dead);
    AddState("SCART_IDLE_CINEMATIC", (statehandler)&TCompSCartController::idleCinematic);

	//ADD MORE STATES FOR BEING HIT, ETC, ETC

	ChangeState("SCART_DISABLED");
}

void TCompSCartController::enable(CHandle vehicle) {
	dbg("Enabling sCart\n");

	vehiclePropHandle = vehicle;
	TCompCollider* player_collider = get<TCompCollider>();
	CEntity* vehicleEntity = vehiclePropHandle.getOwner();
	TCompCollider* vehicle_prop_collider = vehicleEntity->get<TCompCollider>();

	//Get sCart prop position
	if (vehicle_prop_collider->controller != nullptr) {
		isEnabled = true;
		VEC3 prop_pos = PXVEC3_TO_VEC3(vehicle_prop_collider->controller->getPosition());
		VEC3 air = prop_pos;
		air.y += 150.f;
		vehicle_prop_collider->controller->setPosition(VEC3_TO_PXEXVEC3(air));
		//Disable sCart prop collision ( or make it irrelevant, teleporting it to the air)


//Teleport player to sCart position
		TCompTransform* c_trans = get<TCompTransform>();
		if (!player_collider || !player_collider->controller)
			return;
		player_collider->controller->setPosition(VEC3_TO_PXEXVEC3(prop_pos));
		ChangeState("SCART_GROUNDED");
		SwapMesh(1);
		//Generate fake player mounted
		fakePlayerHandle = GameController.spawnPrefab("data/prefabs/props/fake_player_mounted.json", c_trans->getPosition());
	}



}

void TCompSCartController::disable() {
	dbg("Disabling sCart\n");
	isEnabled = false;
	TCompCollider* player_collider = get<TCompCollider>();
	CEntity* vehicleEntity = vehiclePropHandle.getOwner();
	TCompCollider* vehicle_prop_collider = vehicleEntity->get<TCompCollider>();
	TCompTransform* c_trans = get<TCompTransform>();
	TCompCharacterController* cc = get<TCompCharacterController>();

	cc->dismount();
	ChangeState("SCART_DISABLED");

	//Leave sCartProp behind
	//CHANGE TO MESH SWAP
	Vector3 behind = c_trans->getPosition() + (-c_trans->getFront() * 2.f);
	PxTransform pos = vehicle_prop_collider->actor->getGlobalPose();
	float offsetY = 1.5;
	behind.y += offsetY;
	vehicle_prop_collider->controller->setPosition(VEC3_TO_PXEXVEC3(behind));
	SwapMesh(0);
    rowImpulseLeft = 0.f;
	//Remove fake player
	fakePlayerHandle.destroy();
}

void TCompSCartController::disabled() {
	if (isEnabled) {
		dbg("Shopping Cart becomes ENABLED, changes to GROUNDED from DISABLED");
		ChangeState("SCART_GROUNDED");
	}
}

void TCompSCartController::idleCinematic() {
    if (!cinematic) {
        dbg("Shopping Cart becomes ENABLED, changes to GROUNDED from SCART_IDLE_CINEMATIC");
        ChangeState("SCART_GROUNDED");
    }

}


void TCompSCartController::load(const json& j, TEntityParseContext& ctx) {
	this->Init();

	speed = j.value("speed", speed);
	rowDelay = j.value("rowDelay", rowDelay);
	rowImpulse = j.value("rowImpulse", rowImpulse);
	rowImpulseLossRatio = j.value("rowImpulseLossRatio", rowImpulseLossRatio);
	rotation_speed = j.value("rotation_speed", rotation_speed);
}

void TCompSCartController::debugInMenu() {
	ImGui::LabelText("State", "%s", state.c_str());
	ImGui::Checkbox("Gravity Enabled", &gravity_enabled);
	ImGui::DragFloat("Gravity", &gravity, 0.01f, -30.0f, 0.0f);
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 10.f);
	ImGui::DragFloat("Rotation speed", &rotation_speed, 0.1f, 0.f, 10.f);
  ImGui::DragFloat("Impulse", &rowImpulseLeft, 0.1f, 0.f, 100.f);
  
}

void TCompSCartController::registerMsgs() {
	DECL_MSG(TCompSCartController, TMsgOnContact, onCollision);
	DECL_MSG(TCompSCartController, TMsgDamage, onDamage);
    DECL_MSG(TCompSCartController, TMsgOnCinematic, onCinematicScart);
}

void TCompSCartController::onCinematicScart(const TMsgOnCinematic & msg)
{

    cinematic = msg.cinematic;
	UI::CImage* mirilla = dynamic_cast<UI::CImage*>(Engine.getUI().getWidgetByAlias("reticula_"));
	mirilla->getParams()->visible = false;
    if (cinematic) {
        ChangeState("SCART_IDLE_CINEMATIC");
    }
    else {
        if (isEnabled) {
			rowImpulseLeft = 0;//al salir de la cinematica para que no se vaya a cuenca el carrito
            ChangeState("SCART_GROUNDED");
        }
        else {
            ChangeState("SCART_DISABLED");
        }        
    }
}

void TCompSCartController::onCollision(const TMsgOnContact& msg) {

	if (isEnabled) {
		CEntity* source_of_impact = (CEntity *)msg.source.getOwner();
		if (source_of_impact) {
			TCompTags* c_tag = source_of_impact->get<TCompTags>();
			if (c_tag) {
				std::string tag = CTagsManager::get().getTagName(c_tag->tags[0]);
				std::string tag2 = CTagsManager::get().getTagName(c_tag->tags[1]);
				if (strcmp("floor", tag.c_str()) == 0) {

				}
				if (strcmp("cupcake", tag2.c_str()) == 0) {
					if (strcmp("DAMAGED", state.c_str()) != 0) {
						/*CAICupcake* cup = source_of_impact->get<CAICupcake>();
						life -= cup->damage;
						ChangeState("SCART_DAMAGED");
						if (life <= 0.0f) {
							life = 0.0f;
							ChangeState("SCART_DEAD");
						}*/
					}
				}
			}
		}
	}
	else {
		//ChangeState("SCART_DISABLED");
	}
}

void TCompSCartController::onDamage(const TMsgDamage& msg) {
	if (isEnabled) {
		life -= msg.intensityDamage;
		TCompRigidBody* c_rbody = get<TCompRigidBody>();
		if (!c_rbody)
			return;
		EngineAudio.playEvent("event:/Character/Footsteps/Jump_Start");
		
		VEC3 direction_to_damage;
		TCompTransform* my_trans = get<TCompTransform>();
		if (msg.senderType == ENEMIES) { //los enemigos envian el handle
			/*CEntity* entity_to_hit_me = (CEntity *)msg.h_bullet;
			TCompTransform* e_trans = entity_to_hit_me->get<TCompTransform>();
			direction_to_damage = my_trans->getPosition() - e_trans->getPosition();*/
			if(!cinematic) {
				c_rbody->jump(VEC3(0.0f,5.f, 0.0f));
			}
		}
		else { //algunos objetos envian una posicion
			//direction_to_damage = my_trans->getPosition() - msg.position;
			if (!cinematic) {
				c_rbody->jump(VEC3(0.0f, 5.f, 0.0f));
			}
		}
		direction_to_damage.Normalize();
		//c_rbody->addForce(VEC3(direction_to_damage) * 100);
		ChangeState("SCART_DAMAGED");
		if (life <= 0.0f) {
			life = 0.0f;
			//ChangeState("SCART_DEAD");
		}
		
	}
	//else {
	//	ChangeState("SCART_DISABLED");
	//}
}

void  TCompSCartController::SwapMesh(int state) {
	TCompRender* crender = get<TCompRender>();
	crender->showMeshesWithState(state);
}
//End

void TCompSCartController::grounded(float delta) {
	if (!isEnabled) {
		dbg("sCart changes to DISABLED from GROUNDED\n");
		ChangeState("SCART_DISABLED");
		return;
	} 
	else {
		if(EngineInput["interact_"].justPressed()){
			disable();
		}
	}
	//WE NEED THE CAMERA TO BE CREATED TO MOVE FROM ITS PERSPECTIVE
	if (!h_camera.isValid()) {
		h_camera = getEntityByName("PlayerCamera");
		return;
	}
	//SwapMesh(3);


	treatRumble(delta);

  TCompTransform* c_trans = get<TCompTransform>();
  VEC3 dir = c_trans->getFront();
  VEC3 dir_aux = dir;
  TCompRigidBody* r_body = get<TCompRigidBody>();
  if (isGrounded()) {
    VEC3 temp = r_body->ground_normal.Cross(dir);
    dir = temp.Cross(r_body->ground_normal);
  }

  float res = VEC3::Up.Dot(r_body->ground_normal);
  float inclinacion = acosf(res);
  dir.Normalize();
  
  if (dir == VEC3::Zero) {
    dir = dir_aux;
  }

  if(dir.y != 0.0f)
    air_dir = dir;

  if (inclinacion > 0.0f) {//estas en una rampa, deberias hacer cosas
    VEC3 left = VEC3::Up.Cross(r_body->ground_normal);
    VEC3 front = left.Cross(r_body->ground_normal);
    VEC3 front_aux = front;
    front_aux.y = 0.f;
    dir_aux.y = 0.f;
    front_aux.Normalize();
    dir_aux.Normalize();
    float res2 = front_aux.Dot(dir_aux);
    float angle = acosf(res2);
    
    if (abs(angle) <= M_PI / 4.0f) {//Si estas mirando en la direccion que toca, a�adir impulso
      float angleFactor = abs(abs(angle) - M_PI / 4.0f) / (M_PI / 4.0f);
      float inclinationFactor = inclinacion / (M_PI / 4.0f);
      rowImpulseLeft += inclinationRow * angleFactor * inclinationFactor * delta;
      if (rowImpulseLeft > maxImpulse) {
        rowImpulseLeft = maxImpulse;
      }
    }
    else if (abs(M_PI - angle) < M_PI / 3.0f) { //Si estas mirando en la direccion contraria, impulso negativo
      float angleFactor = abs(abs(M_PI - angle) - M_PI / 3.0f) / (M_PI / 3.0f);
      float inclinationFactor = inclinacion / (M_PI / 4.0f);
      rowImpulseLeft -= inclinationRow * angleFactor * inclinationFactor * delta;
    }
  }


	if (rowTimer > 0.0f)
		rowTimer -= delta;
	if (rowImpulseLeft > 0.f) {
		rowImpulseLeft -= rowImpulseLossRatio * delta;
	}
  if (abs(rowImpulseLeft) < 0.01f && r_body->ground_normal == VEC3(0.0f,1.0f, 0.0f)) {
    rowImpulseLeft = 0.0f;
  }

	if (EngineInput["jump_"].justPressed() && rowTimer <= 0.0f) {//ROW
    rowTimer = rowDelay;
		ChangeState("SCART_ROWING");
	}
	else if (!isGrounded() && dir == VEC3()) { //FALLING
    //TRANSMITIR FUERZAS AL RIGID BODY
    air_dir.Normalize();
    r_body->addForce(air_dir * rowImpulseLeft);
		ChangeState("SCART_ON_AIR");
	}

	dir *= delta * rowImpulseLeft;

	//MOVE PLAYER
	TCompCollider* comp_collider = get<TCompCollider>();
	if (!comp_collider || !comp_collider->controller)
		return;

	comp_collider->controller->move(VEC3_TO_PXVEC3(dir), 0.0f, delta, PxControllerFilters());
  //comp_collider->controller->setUpDirection(VEC3_TO_PXVEC3(r_body->ground_normal)); FALTA GIRAR LA MESH EN LA DIRECCION DEL FRONT

	//WHEN CHARACTER IS GROUNDED HE CAN ROTATE
	rotatePlayer(delta);

	//Update fake player
	if (fakePlayerHandle.isValid()) {
		TCompTransform* fake_trans = ((CEntity*)fakePlayerHandle)->get< TCompTransform>();
		fake_trans->setPosition(c_trans->getPosition());
		fake_trans->setRotation(c_trans->getRotation());
	}	
}

void TCompSCartController::rowing(float delta) {
  rowImpulseLeft += rowImpulse;
  if (rowImpulseLeft > maxImpulse) {
    rowImpulseLeft = maxImpulse;
  }
  rowTimer -= delta;
  ChangeState("SCART_GROUNDED");
}

void TCompSCartController::onAir(float delta) {
	//WE NEED THE CAMERA TO BE CREATED TO MOVE FROM ITS PERSPECTIVE
	if (!h_camera.isValid()) {
		h_camera = getEntityByName("Camera");
		return;
	}

  TCompRigidBody* r_body = get<TCompRigidBody>();

	if (isGrounded()) {
		ChangeState("SCART_GROUNDED");
    //get impulse from rigidbody and put it on rowImpulseLeft
    rowImpulseLeft = r_body->getImpulse().Length();
		return;
	}

	treatRumble(delta);

  rotatePlayer(delta);

  //Update fake player
  if (fakePlayerHandle.isValid()) {
	  TCompTransform* c_trans = get<TCompTransform>();
	  TCompTransform* fake_trans = ((CEntity*)fakePlayerHandle)->get< TCompTransform>();
	  fake_trans->setPosition(c_trans->getPosition());
	  fake_trans->setRotation(c_trans->getRotation());
  }
}

void TCompSCartController::damaged(float delta) {
	if (isEnabled && !cinematic) {
		dbg("sCart changes to STATE from DAMAGED\n");
		ChangeState("SCART_GROUNDED");
	}
	else {
		dbg("Error: We shouldn't have entered DAMAGED if we're DISABLED\n");
	}
}

void TCompSCartController::dead(float delta) {
	if (isEnabled) {
		dbg("Disabling sCart from DEAD\n");
		disable();
	}
}

void TCompSCartController::rotatePlayer(float delta) {
  float yaw, pitch;
  TCompTransform* c_trans = get<TCompTransform>();
  c_trans->getAngles(&yaw, &pitch);

  float value = 0.0f;

  if (EngineInput["left_"].isPressed()) {
    value += sensitivity;
  }
  if (EngineInput["right_"].isPressed()) {
    value -= sensitivity;
  }
  //GAMEPAD
  if (EngineInput.gamepad()._connected) {
    //TO CORRECT A BUG WITH GAMEPAD CONNECTED WITH KEYBOARD
    if (!EngineInput["a"].isPressed())
      value -= EngineInput["left_"].value;
  }

  c_trans->setRotation(QUAT::CreateFromYawPitchRoll(yaw + value * rotation_speed * delta, pitch, 0.0f));

}

bool TCompSCartController::isGrounded() {
  TCompRigidBody* r_body = get<TCompRigidBody>();
  return r_body->is_grounded;
}

void TCompSCartController::treatRumble(float delta) {
	rumble_time -= delta;
	if (rumble_time < 0.0f) {
		Input::TRumbleData rumble;
		rumble.leftVibration = 0.0f;
		rumble.rightVibration = 0.0f;
		EngineInput.feedback(rumble);
		rumble_time = 0.0f;
	}
}