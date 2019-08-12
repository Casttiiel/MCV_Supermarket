#pragma once

#include "components/common/comp_base.h"
#include "components/common/comp_name.h"
#include "entity/entity.h"
#include "geometry/curve.h"


class TCompParabolicLaunch : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);//meter en jason la velocidad de lanzamiento
  void debugInMenu();
  void renderDebug();
  void calculateParabolicPoints();
  void update(float dt);
  //h: tranformada y del pj, angulo de lanzamiento
  void loadValuesParabola(float new_h, float pitch,float yaw);
  float h;
  float pitch;
  float yaw;
  float v0x; //componente z
  float v0y; //componente y
  CCurve* curve_dynamic = nullptr; //guardamos vector de puntos de punto maximo y alcance
  bool paintCurve = false;
  CHandle h_player;
  CHandle e_player;

private:
	float heightPj = 1.80f;
	int nsamples =100;
	float v0 = 20.f;//velocidad en modulo
	MAT44 _curveTransform;
	
	

};

