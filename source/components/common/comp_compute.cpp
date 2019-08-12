#include "mcv_platform.h"
#include "comp_compute.h"
#include "comp_buffers.h"
#include "render/compute/compute_shader.h"
#include "render/compute/gpu_buffer.h"
#include "comp_num_instances.h"
#include "engine.h"
#include "render/module_gpu_culling.h"

DECL_OBJ_MANAGER("compute", TCompCompute);

void TCompCompute::debugInMenu() {
  ImGui::DragInt3("Sizes", (int*) sizes, 0.1f, 1, 1024);
  uint32_t args[3];
  getDispatchArgs(args);
  ImGui::LabelText("Args", "%d %d %d", args[0], args[1], args[2]);
  ((CComputeShader*)compute)->renderInMenu();
}

void TCompCompute::load(const json& j, TEntityParseContext& ctx) {

  // Compute shader
  std::string cs = j.value("cs", "");
  compute = Resources.get(cs)->as<CComputeShader>();

  // Invocation default sizes
  const json& jsizes = j["sizes"];
  sizes[0] = jsizes[0].get<int>();
  sizes[1] = jsizes[1].get<int>();
  sizes[2] = jsizes[2].get<int>();
}

void TCompCompute::bindArguments(TCompBuffers* c_buffers) {

  assert(compute);
  UINT zeros = 0;
  for (auto it : compute->bound_resources) {
    switch (it.Type) {

    case D3D_SIT_CBUFFER: {
      CCteBufferBase* cte = nullptr;
      if (strcmp(it.Name, "TCtesShared") == 0) {
        cte = &ctes_shared;
      } else if (strcmp(it.Name, "TCtesCamera") == 0) {
        cte = &ctes_camera;
      } else {
        cte = c_buffers->getCteByName(it.Name);
      }
      if (cte == nullptr)
        fatal("Don't know how to bind cte buffer called %s\n", it.Name);
      cte->activateInCS(it.BindPoint);
      break; }

    case D3D_SIT_UAV_RWBYTEADDRESS:
    case D3D_SIT_UAV_RWSTRUCTURED: {
      assert(c_buffers);
      CGPUBuffer* buf = c_buffers->getBufferByName(it.Name);
      assert(buf);
      assert(buf->uav);
      Render.ctx->CSSetUnorderedAccessViews(it.BindPoint, 1, &buf->uav, &zeros);
      break; }

    case D3D_SIT_STRUCTURED: {
      assert(c_buffers);
      CGPUBuffer* buf = c_buffers->getBufferByName(it.Name);
      assert(buf);
      assert(buf->srv);
      Render.ctx->CSSetShaderResources(it.BindPoint, 1, &buf->srv);
      break; }

    default:
      fatal("Don't know how to bind a compute shader arg of type %d named %s\n", it.Type, it.Name);
      break;

      //D3D_SIT_TBUFFER = (D3D_SIT_CBUFFER + 1),
      //D3D_SIT_TEXTURE = (D3D_SIT_TBUFFER + 1),
      //D3D_SIT_SAMPLER = (D3D_SIT_TEXTURE + 1),
      //D3D_SIT_UAV_RWTYPED = (D3D_SIT_SAMPLER + 1),
      //D3D_SIT_STRUCTURED = (D3D_SIT_UAV_RWTYPED + 1),
      //D3D_SIT_BYTEADDRESS = (D3D_SIT_UAV_RWSTRUCTURED + 1),
      //D3D_SIT_UAV_RWBYTEADDRESS = (D3D_SIT_BYTEADDRESS + 1),
      //D3D_SIT_UAV_APPEND_STRUCTURED = (D3D_SIT_UAV_RWBYTEADDRESS + 1),
      //D3D_SIT_UAV_CONSUME_STRUCTURED = (D3D_SIT_UAV_APPEND_STRUCTURED + 1),
      //D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER = (D3D_SIT_UAV_CONSUME_STRUCTURED + 1),
    }
  }
}

bool TCompCompute::getDispatchArgs(uint32_t* args) {

  for (int i = 0; i < 3; ++i) {
    args[i] = sizes[i] / compute->thread_group_size[i];
    if (args[i] * compute->thread_group_size[i] < sizes[i])
      args[i]++;
  }

  if (args[0] == 0 || args[1] == 0 || args[2] == 0)
    return false;
  return true;
}

// Global loop to update all registered compute shaders
void CObjectManager< TCompCompute > ::updateAll(float dt) {
  CGpuScope gpu_scope("Compute Shaders");

  // Can't update a buffer if it's still bound as vb. So unbound it.
  CVertexShader::deactivateResources();

  Engine.getGPUCulling().run();

  for (uint32_t i = 0; i < num_objs_used; ++i)
    objs[i].update(dt);

  CComputeShader::deactivate();
}

void TCompCompute::run(TCompBuffers* c_buffers) {
  CGpuScope gpu_scope(compute->getName().c_str());
  uint32_t args[3];
  if (!getDispatchArgs(args))
    return;
  compute->activate();
  bindArguments(c_buffers);
  Render.ctx->Dispatch(args[0], args[1], args[2]);
}

void TCompCompute::update(float delta) {
  TCompBuffers* c_buffers = get<TCompBuffers>();
 
  run(c_buffers);

  // Update sibling component
  TCompNumInstances* c_num_instances = get<TCompNumInstances>();
  if (c_num_instances)
    c_num_instances->num_instances = sizes[0] * sizes[1] * sizes[2];
}


