#include "mcv_platform.h"
#include "comp_enemies_in_tube.h"
#include "components/controllers/character/comp_character_controller.h"
#include "modules/game/module_game_controller.h"
#include "engine.h"
#include "components/ai/bt/bt_cupcake.h"
DECL_OBJ_MANAGER("enemies_in_tube", TCompEnemiesInTube);

void TCompEnemiesInTube::debugInMenu() {

}


void TCompEnemiesInTube::registerMsgs() {
  DECL_MSG(TCompEnemiesInTube, TMSgWallDestroyed, onFinishEnemiesInTube);
}

void TCompEnemiesInTube::onFinishEnemiesInTube(const TMSgWallDestroyed& msg) {
  if (msg.isDetroyed) {
    activateTrap = false;
    CHandle(this).getOwner().destroy();
    CHandle(this).destroy();
  }
}


void TCompEnemiesInTube::load(const json& j, TEntityParseContext& ctx) {
  enemiesPosition.push_back(VEC3(18.214, 9.143, -7.335));
  enemiesPosition.push_back(VEC3(27.714, 9.143, -7.335));
  enemiesPosition.push_back(VEC3(27.714, 9.143, 5.790));
  enemiesPosition.push_back(VEC3(18.064, 9.143, 5.890));
  /*
  h1 = GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
  h2 = GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
  h3 = GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
  h4 = GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);
  */

}


void TCompEnemiesInTube::update(float dt) {

  if (activateTrap) {
    if (birthTime > 0.0f) {
      birthTime -= dt;
      if (birthTime < 4.5f) {
        CEntity* entity_emis1 = (CEntity*)h1;
        if (entity_emis1 != nullptr) {
          TCompBuffers* c_buff1 = entity_emis1->get<TCompBuffers>();
          if (c_buff1) {
            auto buf1 = c_buff1->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data1 = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf1);
            data1->emitter_num_particles_per_spawn = 5;
            data1->updateGPU();

          }
        }
        CEntity* entity_emis2 = (CEntity*)h2;
        if (entity_emis2 != nullptr) {
          TCompBuffers* c_buff2 = entity_emis2->get<TCompBuffers>();
          if (c_buff2) {
            auto buf2 = c_buff2->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data2 = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf2);
            data2->emitter_num_particles_per_spawn = 5;
            data2->updateGPU();
          }
        }
        CEntity* entity_emis3 = (CEntity*)h3;
        if (entity_emis3 != nullptr) {
          TCompBuffers* c_buff3 = entity_emis3->get<TCompBuffers>();
          if (c_buff3) {
            auto buf3 = c_buff3->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data3 = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf3);
            data3->emitter_num_particles_per_spawn = 5;
            data3->updateGPU();
          }
        }
        CEntity* entity_emis4 = (CEntity*)h4;
        if (entity_emis4 != nullptr) {
          TCompBuffers* c_buff4 = entity_emis4->get<TCompBuffers>();
          if (c_buff4) {
            auto buf4 = c_buff4->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data4 = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf4);
            data4->emitter_num_particles_per_spawn = 5;
            data4->updateGPU();
          }
        }
      }
    }
    else {

      if (positionBirth == 0) {
        if (!activateTrap1) {
          h1 = GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
          activateTrap1 = true;
        }
        else {
          CEntity* entity_emis = (CEntity*)h1;
          TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
          if (c_buff) {
            auto buf = c_buff->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
            data->emitter_num_particles_per_spawn = 50;
            data->updateGPU();
          }
        }
      }
      if (positionBirth == 1) {
        if (!activateTrap2) {
          h2 = GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
          activateTrap2 = true;
        }
        else {
          CEntity* entity_emis = (CEntity*)h2;
          TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
          if (c_buff) {
            auto buf = c_buff->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
            data->emitter_num_particles_per_spawn = 50;
            data->updateGPU();
          }
        }
      }
      if (positionBirth == 2) {
        if (!activateTrap3) {
          h3 = GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
          activateTrap3 = true;
        }
        else {
          CEntity* entity_emis = (CEntity*)h3;
          TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
          if (c_buff) {
            auto buf = c_buff->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
            data->emitter_num_particles_per_spawn = 50;
            data->updateGPU();
          }
        }
      }
      if (positionBirth == 3) {
        if (!activateTrap4) {
          h4 = GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);
          activateTrap4 = true;
        }
        else {
          CEntity* entity_emis = (CEntity*)h4;
          TCompBuffers* c_buff = entity_emis->get<TCompBuffers>();
          if (c_buff) {
            auto buf = c_buff->getCteByName("TCtesParticles");
            CCteBuffer<TCtesParticles>* data = dynamic_cast<CCteBuffer<TCtesParticles>*>(buf);
            data->emitter_num_particles_per_spawn = 50;
            data->updateGPU();
          }
        }
      }
      CHandle handle = GameController.spawnPrefab("data/prefabs/enemies/bt_cupcake.json", enemiesPosition[positionBirth], QUAT(0, 0, 0, 1), 1);

      if (!firstTime) {
        /*GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);*/
        firstTime = true;
      }

      CEntity* entity = (CEntity*)handle;
      CBTCupcake* cupcake = entity->get<CBTCupcake>();
      cupcake->setHalfCone(360);
      cupcake->setLengthCone(300);
      positionBirth++;
      if (positionBirth >= enemiesPosition.size()) {
        positionBirth = 0;
      }
      birthTime = birthTimeReset;
    }
  }

}



//CODIGO ANTIGUO

/*
#include "mcv_platform.h"
#include "comp_enemies_in_tube.h"
#include "components/controllers/character/comp_character_controller.h"
#include "modules/game/module_game_controller.h"
#include "engine.h"
#include "components/ai/bt/bt_cupcake.h"
DECL_OBJ_MANAGER("enemies_in_tube", TCompEnemiesInTube);

void TCompEnemiesInTube::debugInMenu() {

}


void TCompEnemiesInTube::registerMsgs() {
  DECL_MSG(TCompEnemiesInTube, TMSgWallDestroyed, onFinishEnemiesInTube);
}

void TCompEnemiesInTube::onFinishEnemiesInTube(const TMSgWallDestroyed & msg) {
  if (msg.isDetroyed) {
    activateTrap = false;
    CHandle(this).getOwner().destroy();
    CHandle(this).destroy();
  }
}


void TCompEnemiesInTube::load(const json& j, TEntityParseContext& ctx){
  enemiesPosition.push_back(VEC3(18.214,9.143,-7.335));
  enemiesPosition.push_back(VEC3(27.714, 9.143, -7.335));
  enemiesPosition.push_back(VEC3(27.714, 9.143, 5.790));
  enemiesPosition.push_back(VEC3(18.064, 9.143, 5.890));



}


void TCompEnemiesInTube::update(float dt) {
  if(activateTrap){
    if (birthTime > 0) {
      birthTime -= dt;
    }
    else {
      CHandle handle = GameController.spawnPrefab("data/prefabs/enemies/bt_cupcake.json", enemiesPosition[positionBirth], QUAT(0, 0, 0, 1), 1);
      if(!firstTime){
        GameController.spawnPrefab("data/particles/smoke_tube1.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube2.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube3.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        GameController.spawnPrefab("data/particles/smoke_tube4.json", VEC3(), QUAT(0, 0, 0, 1), 1);
        firstTime = true;
      }

      CEntity* entity = (CEntity *)handle;
      CBTCupcake* cupcake = entity->get<CBTCupcake>();
      cupcake->setHalfCone(360);
      cupcake->setLengthCone(300);
      positionBirth++;
      if (positionBirth >= enemiesPosition.size() ) {
        positionBirth = 0;
      }
      birthTime = birthTimeReset;
    }
  }
}

*/