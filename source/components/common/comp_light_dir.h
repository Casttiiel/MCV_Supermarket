#pragma once

#include "comp_base.h"
#include "comp_camera.h"
#include "entity/common_msgs.h"

class CTexture;
class CRenderToTexture;

class TCompLightDir : public TCompCamera {

  // Light params
  VEC4            color = VEC4(1, 1, 1, 1);
  float           intensity = 1.0f;
  const CTexture* projector = nullptr;

  // Shadows params
  bool              shadows_enabled = false;    // Dynamic
  bool              casts_shadows = false;      // Static
  bool              AABBpassed = true;
  int               shadows_resolution = 256;
  float             shadows_step = 1.f;
  CRenderToTexture* shadows_rt = nullptr;
  VEC3              offset;
  CHandle           e_player;  

  void onDefineLocalAABB(const TMsgDefineLocalAABB& msg);


public:
  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  static void registerMsgs();
  DECL_SIBLING_ACCESS();

  void activate();
  void generateShadowMap();

  bool isShadowEnabled() { return shadows_enabled; }

};