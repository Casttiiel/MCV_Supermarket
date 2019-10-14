#include "mcv_platform.h"
#include "comp_onom_manager.h"
#include "engine.h"
#include "components/common/comp_transform.h"


DECL_OBJ_MANAGER("onom_manager", TCompOnomManager);

void TCompOnomManager::debugInMenu() {
}

void TCompOnomManager::load(const json& j, TEntityParseContext& ctx) {
}

void TCompOnomManager::renderDebug() {
}

void TCompOnomManager::registerMsgs() {
  DECL_MSG(TCompOnomManager, TMsgOnomPet, onPetition);
}

void TCompOnomManager::onPetition(const TMsgOnomPet& msg) {
  if (!camera.isValid())
    return;
  //if the petition is not a boom, we calculate a better position based on camera pos
  if (msg.type != 3) {
    TMsgOnomPet newmsg;
    if (msg.type == 2 && wham) {
      newmsg.type = 2.0f;
      wham = false;
    }
    else if (msg.type == 2 && !wham) {
      newmsg.type = 4.0f;
      wham = true;
    }
    else {
      newmsg.type = 1.0f;
    }

    CEntity* e_cam = camera;
    TCompTransform* c_trans = e_cam->get<TCompTransform>();
    newmsg.pos = msg.pos + (VEC3::Up * 1.2f) - (c_trans->getLeft() * 0.5f) + (c_trans->getFront() * 0.25f);

    petitions.push_back(newmsg);
  }
  else {
    petitions.push_back(msg);
  }
  
}

void TCompOnomManager::update(float delta) {
  if (!camera.isValid()) {
    camera = getEntityByName("PlayerCamera");
    return;
  }

  TCompBuffers* c_buff = get<TCompBuffers>();
  if (c_buff) {
    auto buf = c_buff->getCteByName("TCtesParticles");
    CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
    if (petitions.size() > 0) {
      data->emitter_num_particles_per_spawn = 1;
      data->emitter_center = petitions[0].pos;
      data->emitter_dir_aperture = petitions[0].type;
      /*if (petitions[0].type == 1.0f) {
        //dbg("zapit\n");
      }*/
      //erase first element of petitions vector
      petitions.erase(petitions.begin());
    } else {
      data->emitter_num_particles_per_spawn = 0;
      data->emitter_dir_aperture = 0;
    }
    data->updateGPU();
  }
}
