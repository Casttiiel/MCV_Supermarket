#ifndef INC_COMP_SKEL_LOOKAT_DIR_
#define INC_COMP_SKEL_LOOKAT_DIR_

#include "components/common/comp_base.h"
#include "entity/entity.h"
#include "bone_correction.h"

class CalModel;
struct TCompSkeleton;

struct TCompSkelLookAtDirection : public TCompBase {
  CHandle     h_skeleton;        // Handle to comp_skeleton of the entity being tracked
  CHandle     h_target_entity;
  std::string target_entity_name;

  VEC3        target = VEC3().Zero;
  float       amount = 0.f;
  float       target_transition_factor = 0.95f;
  VEC3        dir;
  VEC3        default_dir = VEC3().Zero;
  float       positionOffset = 20.f;

  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void debugInMenu();
  void renderDebug();

  void setDirection(VEC3 direction);
  void setDefaultDirection(VEC3 defaultDirection);
  DECL_SIBLING_ACCESS();
};

#endif
