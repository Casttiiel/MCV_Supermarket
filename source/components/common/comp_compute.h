#pragma once

#include "comp_base.h"
#include "entity/entity.h"

class CComputeShader;
class CGPUBuffer;
struct TCompBuffers;
 
struct TCompCompute : public TCompBase {

  const CComputeShader*          compute = nullptr;

  uint32_t sizes[3];

  void bindArguments(TCompBuffers* c_buffers);
  bool getDispatchArgs(uint32_t* w);
  void run(TCompBuffers* c_buffers);

public:

  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();
  void update(float dt);

  DECL_SIBLING_ACCESS();
};