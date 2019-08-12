#include "mcv_platform.h"
#include "comp_buffers.h"
#include "render/compute/gpu_buffer.h"

DECL_OBJ_MANAGER("buffers", TCompBuffers);

TCompBuffers::~TCompBuffers() {
  for (auto& b : gpu_buffers)
    b->destroy();
  gpu_buffers.clear();

  for (auto& b : cte_buffers)
    b->destroy();
  cte_buffers.clear();
}

void TCompBuffers::debugInMenu() {
  for (auto b : gpu_buffers)
    ImGui::Text("GPU Buffer %s (%ld bytes)", b->name.c_str(), b->cpu_data.size());
  for (auto b : cte_buffers)
    b->debugInMenu();
}

CGPUBuffer* TCompBuffers::getBufferByName(const char* name) {
  for (auto b : gpu_buffers) {
    if (strcmp(b->name.c_str(), name) == 0)
      return b;
  }
  return nullptr;
}

CCteBufferBase* TCompBuffers::getCteByName(const char* name) {
  for (auto b : cte_buffers) {
    if (strcmp(b->getName().c_str(), name) == 0)
      return b;
  }
  return nullptr;
}

void TCompBuffers::load(const json& j, TEntityParseContext& ctx) {

  // Owned buffers by me
  for (auto jit : j.items()) {
    const std::string& key = jit.key();
    json jval = jit.value();
    jval["name"] = key;

    if (jval.count("total_bytes")) {
      CCteRawBuffer* cte_buffer = new CCteRawBuffer();
      uint32_t slot_idx = jval.value("slot", -1);
      uint32_t total_bytes = jval.value("total_bytes", 0);
      cte_buffer->create(total_bytes, key.c_str(), slot_idx);
      assert(cte_buffer->size() == total_bytes);
      cte_buffers.push_back(cte_buffer);
      continue;
    }

    {
      CGPUBuffer* gpu_buffer = new CGPUBuffer();
      bool is_ok = gpu_buffer->create(jval);
      assert(is_ok || fatal("Failed to create compute shader buffer %s\n", key.c_str()));
      gpu_buffers.push_back(gpu_buffer);
    }
  }

}

