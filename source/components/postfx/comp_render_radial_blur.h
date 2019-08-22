#ifndef INC_COMPONENT_RENDER_RADIAL_BLUR_H_
#define INC_COMPONENT_RENDER_RADIAL_BLUR_H_

#include "components/common/comp_base.h"
#include <vector>

struct CRadialBlurStep;
class  CTexture;

// ------------------------------------
struct TCompRenderRadialBlur : public TCompBase {
  std::vector< CRadialBlurStep* > steps;
  VEC4  weights;
  VEC4  distance_factors;     // 1 2 3 4
  float global_distance;
  int   nactive_steps;
  bool enabled;
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();
  CTexture* apply(CTexture* in_texture);
};

#endif
