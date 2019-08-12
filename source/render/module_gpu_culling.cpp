#include "mcv_platform.h"
#include "module_gpu_culling.h"
#include "render/compute/gpu_buffer.h"
#include "components/common/comp_camera.h"
#include "components/common/comp_light_dir.h"
#include "components/common/comp_transform.h"
#include "components/common/comp_render.h"
#include "components/common/comp_lod.h"
#include "render/textures/material.h"
#include "utils/utils.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "render/compute/compute_shader.h"

AABB getRotatedBy(AABB src, const MAT44 &model);

struct TSampleDataGenerator {
  CModuleGPUCulling* mod = nullptr;

  VEC3         pmin;
  VEC3         pmax;
  float        scale = 1.0f;
  uint32_t     num_instances = 0;

  VHandles     prefabs;

  void create(const json& j) {

    float radius = j.value("radius", 30.f);
    pmin = VEC3(-radius, 0.f, -radius);
    pmax = VEC3(radius, 1.0f, radius);
    num_instances = j.value("num_instances", num_instances);

    std::vector< std::string > prefab_names = j["prefabs"].get< std::vector< std::string > >();
    for (auto& prefab_name : prefab_names) {
      TEntityParseContext ctx;
      bool is_ok = parseScene(prefab_name, ctx);
      assert(is_ok);
      prefabs.push_back( ctx.entities_loaded[0] );
    }

    generate();
  }

  void generate() {

    assert(mod);
    assert(!prefabs.empty());

    for (uint32_t i = 0; i < num_instances; ++i) {

      // Right now, always choose the first prefab
      int idx = rand() % prefabs.size();

      // Access the prefab recently created
      CHandle prefab = prefabs[idx];
      CEntity* e = prefab;
      assert(e);

      // Find a random prefab
      VEC3 center = pmin + (pmax - pmin) * VEC3(unitRandom(), unitRandom(), unitRandom());
      float sc = scale;

      // Take original transform of the prefab (may contain rotation & scale already)
      TCompTransform* c_transform = e->get< TCompTransform >();

      // Add a random yaw
      MAT44 rot_yaw = MAT44::CreateFromAxisAngle(VEC3(0,1,0), unitRandom() * deg2rad(360.0f));

      MAT44 world = c_transform->asMatrix() * rot_yaw * MAT44::CreateScale(sc) * MAT44::CreateTranslation(center);

      // Find AABB in world space
      TCompRender* cr = e->get< TCompRender >();
      const TCompRender::MeshPart& mp = cr->parts[1];       // Use group 1 -> bark
      AABB aabb_local = mp.mesh->getAABB();
      AABB aabb_abs = getRotatedBy(aabb_local, world);
      
      // Register object & matrix to be rendered
      mod->addToRender(prefab, aabb_abs, world);
    }

    // Delete the prefabs we have been using to create the fake data
    for( auto h_prefab : prefabs )
      h_prefab.destroy();
    prefabs.clear();
  }
};

// ---------------------------------------------------------------
bool CModuleGPUCulling::start() {

  json j = loadJson("data/gpu_culling.json");

  show_debug = j.value("show_debug", show_debug);

  TEntityParseContext ctx;
  comp_compute.load(j["compute"], ctx);
  comp_buffers.load(j["buffers"], ctx);

  // Access buffer to hold the instances in the gpu
  gpu_objs = comp_buffers.getBufferByName("instances");
  assert(gpu_objs || fatal("Missing required buffer to hold the instances to be culled\n"));
  max_objs = gpu_objs->num_elems;
  assert(gpu_objs->bytes_per_elem == sizeof(TObj) || fatal("GPU/CPU struct size don't match for instances %d vs %d\n", gpu_objs->bytes_per_elem, (uint32_t)sizeof(TObj)));

  auto gpu_culled_instances = comp_buffers.getBufferByName("culled_instances");
  assert(gpu_culled_instances);
  assert(gpu_culled_instances->bytes_per_elem == sizeof(MAT44) || fatal("GPU/CPU struct size don't match for culled_instances %d vs %d\n", gpu_culled_instances->bytes_per_elem, (uint32_t)sizeof(MAT44)));

  assert(comp_buffers.getCteByName("TCullingPlanes")->size() == sizeof(TCullingPlanes));

  gpu_ctes_instancing = comp_buffers.getCteByName("TCtesInstancing");
  assert(gpu_ctes_instancing);
  assert(gpu_ctes_instancing->size() == sizeof(TCtesInstancing));

  gpu_prefabs = comp_buffers.getBufferByName("prefabs");
  max_prefabs = gpu_prefabs->num_elems;
  assert(gpu_prefabs->bytes_per_elem == sizeof(TPrefab) || fatal("GPU/CPU struct size don't match for prefabs %d vs %d\n", gpu_prefabs->bytes_per_elem, (uint32_t)sizeof(TPrefab)));

  gpu_draw_datas = comp_buffers.getBufferByName("draw_datas");
  max_render_types = gpu_draw_datas->num_elems;
  assert(gpu_draw_datas->bytes_per_elem == sizeof(TDrawData) || fatal("GPU/CPU struct size don't match for draw_datas %d vs %d\n", gpu_draw_datas->bytes_per_elem, (uint32_t)sizeof(TDrawData)));

  // Reserve in CPU all the memory that we might use, so when we upload cpu data to gpu, we read from valid memory
  // as we upload/read the full buffer size.
  objs.reserve(max_objs);
  prefabs.reserve(max_prefabs);
  render_types.reserve(max_render_types);
  draw_datas.reserve(max_render_types);

  // populate with some random generated data
  /*TSampleDataGenerator sample_data; //HERE WE GENERATE THE TREES
  sample_data.mod = this;
  sample_data.create(j["sample_data"]);*/

  entity_camera_name = j["camera"];
  assert(!entity_camera_name.empty());

  return true;
}

void CModuleGPUCulling::stop() {
  objs.clear();
}

// ---------------------------------------------------------------
// Returns the index position in the array of prefabs 
uint32_t CModuleGPUCulling::addPrefabInstance(CHandle new_id) {
  uint32_t idx = 0;
  for (auto& prefab : prefabs) {
    if (prefab.id == new_id) {
      prefab.num_objs++;
      return idx;
    }
    ++idx;
  }
  idx = registerPrefab(new_id);
  prefabs[idx].num_objs++;
  return idx;
}

uint32_t CModuleGPUCulling::registerPrefab(CHandle new_id) {

  // Register 
  TPrefab prefab;
  prefab.id = new_id;
  prefab.num_objs = 0;
  prefab.num_render_type_ids = 0;

  // Check how many draw calls (instance_types) we have
  CEntity* e = new_id;
  assert(e);
  TCompRender* cr = e->get<TCompRender>();
  assert(cr);
  for (auto& p : cr->parts) {

    // This is the identifier of a draw call
    TRenderTypeID tid;
    tid.group = p.mesh_group;
    tid.mesh = p.mesh;
    tid.material = p.material;

    // Find which type of instance type is this draw call
    uint32_t render_type_id = addRenderType(tid);

    // Save it
    assert(prefab.num_render_type_ids < TPrefab::max_render_types_per_prefab);
    prefab.render_type_ids[prefab.num_render_type_ids] = render_type_id;
    ++prefab.num_render_type_ids;
  }

  assert(prefabs.size() + 1 < max_prefabs || fatal( "We need more space in the gpu buffer 'prefabs'. Current size is %d\n", max_prefabs));
  prefabs.push_back(prefab);
  uint32_t idx = (uint32_t)prefabs.size() - 1;

  // Register a lod if exists a complod in the hi-quality prefab
  TCompLod* c_lod = e->get<TCompLod>();
  if (c_lod) {

    // Load the low-quality prefab
    TEntityParseContext ctx;
    bool is_ok = parseScene(c_lod->replacement_prefab, ctx);
    CHandle h_lod = ctx.entities_loaded[0];

    uint32_t lod_idx = registerPrefab(h_lod);
    setPrefabLod(idx, lod_idx, c_lod->threshold);

    h_lod.destroy();
  }

  return idx;
}

// ---------------------------------------------------------------
void CModuleGPUCulling::setPrefabLod(uint32_t high_prefab_idx, uint32_t low_prefab_idx, float threshold) {
  assert(high_prefab_idx < prefabs.size());
  TPrefab& hq = prefabs[high_prefab_idx];
  hq.lod_prefab = low_prefab_idx;
  hq.lod_threshold = threshold;
}

// ---------------------------------------------------------------
uint32_t CModuleGPUCulling::addRenderType(const TRenderTypeID& new_render_type) {

  uint32_t idx = 0;
  for (auto& render_type : render_types) {
    if (render_type == new_render_type)
      return idx;
    ++idx;
  }

  assert((render_types.size() + 1 < max_render_types) || fatal("Too many (%d) render_types registered. The GPU Buffer 'draw_datas' need to be larger.\n", max_render_types));

  // Register. Copy the key
  render_types.push_back(new_render_type);

  // Create a new name for the prefab
  std::string mesh_name = new_render_type.mesh->getName();
  auto off = mesh_name.find_last_of("/");
  mesh_name = mesh_name.substr(off);
  std::string mat_name = new_render_type.material->getName();
  off = mat_name.find_last_of("/");
  mat_name = mat_name.substr(off);
  snprintf(render_types.back().title, sizeof(TRenderTypeID::title), "%s:%d %s", mesh_name.c_str(), new_render_type.group, mat_name.c_str());

  // Collect the range of triangles we need to render
  TDrawData draw_data = {};
  const TMeshGroup& g = new_render_type.mesh->getGroups()[new_render_type.group];
  draw_data.args.indexCount = g.num_indices;
  draw_data.args.firstIndex = g.first_idx;
  draw_datas.push_back(draw_data);

  return idx;
}

// ---------------------------------------------------------------
void CModuleGPUCulling::addToRender(
  CHandle h_prefab
, const AABB aabb
, const MAT44 world
) {

  assert((objs.size()+1 < max_objs) || fatal("Too many (%d) instances registered. The GPU Buffers 'gpu_instances' need to be larger.\n", max_objs));

  TObj obj;
  obj.aabb_center = aabb.Center;
  obj.prefab_idx = addPrefabInstance(h_prefab);
  obj.aabb_half = aabb.Extents;
  obj.world = world;
  objs.push_back(obj);

  is_dirty = true;
}

// ---------------------------------------------------------------
void CModuleGPUCulling::renderDebug() {
  PROFILE_FUNCTION("GPUCulling");
  if (show_debug) {
    for (auto& obj : objs)
      drawWiredAABB(obj.aabb_center, obj.aabb_half, MAT44::Identity, VEC4(1, 0, 0, 1));
  }
}

void CModuleGPUCulling::update( float delta ) {
  if (!h_camera.isValid()) {
    h_camera = getEntityByName(entity_camera_name);
    if (!h_camera.isValid())
      return;
  }

  CEntity* e_camera = h_camera;
  TCompCamera* c_camera = e_camera->get<TCompCamera>();
  if (!c_camera) {
    TCompLightDir* c_light_dir = e_camera->get<TCompLightDir>();
    assert(c_light_dir);
    culling_camera = *(CCamera*)c_light_dir;
  }
  else {
    culling_camera = *(CCamera*)c_camera;
  }

  updateCullingPlanes(culling_camera);
}

// ---------------------------------------------------------------
void CModuleGPUCulling::updateCullingPlanes(const CCamera& camera) {
  MAT44 m = camera.getViewProjection().Transpose();
  VEC4 mx(m._11, m._12, m._13, m._14);
  VEC4 my(m._21, m._22, m._23, m._24);
  VEC4 mz(m._31, m._32, m._33, m._34);
  VEC4 mw(m._41, m._42, m._43, m._44);
  culling_planes.planes[0] = (mw + mx);
  culling_planes.planes[1] = (mw - mx);
  culling_planes.planes[2] = (mw + my);
  culling_planes.planes[3] = (mw - my);
  culling_planes.planes[4] = (mw + mz);      // + mz if frustum is 0..1
  culling_planes.planes[5] = (mw - mz);
  culling_planes.CullingCameraPos = camera.getPosition();
}

// ---------------------------------------------------------------
void CModuleGPUCulling::renderInMenu() {
  if (ImGui::TreeNode("GPU Culling")) {
    ImGui::Text("%ld objects", (uint32_t)objs.size());
    ImGui::Checkbox("Show Debug", &show_debug);

    if (ImGui::TreeNode("Objs")) {
      for (auto& obj : objs) 
        ImGui::Text("Prefab:%d at %f %f %f", obj.prefab_idx, obj.aabb_center.x, obj.aabb_center.y, obj.aabb_center.z);
      ImGui::TreePop();
    }


    if (ImGui::TreeNode("Prefabs")) {
      int idx = 0;
      for (auto& p : prefabs) {
        ImGui::Text("[%d] %3d num_objs with %d render types Lod:%d at %f Total:%d", idx, p.num_objs, p.num_render_type_ids, p.lod_prefab, p.lod_threshold, p.total_num_objs);
        if (ImGui::TreeNode("Render Type IDs")) {
          for (uint32_t i = 0; i < p.num_render_type_ids; ++i)
            ImGui::Text("[%d] %d", i, p.render_type_ids[i]);
          ImGui::TreePop();
        }
        ++idx;
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Render Types")) {
      int idx = 0;
      for (auto& rt : render_types) {
        ImGui::Text("[%d] %d M:%s Mat:%s", idx, rt.group, rt.mesh->getName().c_str(), rt.material->getName().c_str());
        ++idx;
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Draw Datas")) {
      for (auto& dd: draw_datas)
        ImGui::Text("Base:%3d indices:%4d from:%d", dd.base, dd.args.indexCount, dd.args.firstIndex);
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("GPU Draw Datas")) {
      gpu_draw_datas->copyGPUtoCPU();
      TDrawData* dd = (TDrawData*)gpu_draw_datas->cpu_data.data();
      for (uint32_t i = 0; i < draw_datas.size(); ++i, ++dd ) {
        ImGui::Text("Base:%3d Draw Instances:%3d #Idxs:%4d From:%d Dummy:%d,%d", dd->base, dd->args.instanceCount, dd->args.indexCount, dd->args.firstIndex,dd->dummy[0], dd->dummy[1]);
      }
      ImGui::TreePop();
    }
    ImGui::TreePop();
  }
}

// ---------------------------------------------------------------
// Every frame, we need to clear the #instances in the gpu
// We could do that in a cs also.
void CModuleGPUCulling::clearRenderDataInGPU() {

  for (auto& dd : draw_datas) 
    dd.args.instanceCount = 0;

  // This line has to be done at least once
  gpu_draw_datas->copyCPUtoGPUFrom(draw_datas.data());
}

// ---------------------------------------------------------------
void CModuleGPUCulling::preparePrefabs() {

  // Count my official number of objects
  for (auto& p : prefabs)
    p.total_num_objs = p.num_objs;

  // Then, if I can become a lod
  for (auto& p : prefabs) {
    if (p.lod_prefab != -1) {
      auto& p_low = prefabs[p.lod_prefab];
      assert((p_low.lod_prefab == -1) || fatal( "We don't support yet, lod of lod"));
      p_low.total_num_objs += p.total_num_objs;
    }
  }

  uint32_t base = 0;
  for (auto& p : prefabs) {
    // Each prefab will render 
    for (uint32_t idx = 0; idx < p.num_render_type_ids; ++idx) {
      uint32_t render_type_id = p.render_type_ids[idx];
      draw_datas[render_type_id].base = base;
      base += p.total_num_objs;
    }
  }

  uint32_t max_culled_instances = comp_buffers.getBufferByName("culled_instances")->num_elems;
  assert((base <= max_culled_instances) || fatal("We require more space in the buffer %d. Current %d", base, max_culled_instances));

  assert(gpu_prefabs);
  gpu_prefabs->copyCPUtoGPUFrom(prefabs.data());
}

// ---------------------------------------------------------------
// This is called when we run the Compute Shaders
void CModuleGPUCulling::run() {
  CGpuScope gpu_scope("GPU Culling");

  // Upload culling planes to GPU
  comp_buffers.getCteByName("TCullingPlanes")->updateGPU(&culling_planes);

  if (is_dirty) {
    gpu_objs->copyCPUtoGPUFrom(objs.data());

    preparePrefabs();

    // Notify total number of objects we must try to cull
    ctes_instancing.total_num_objs = (uint32_t) objs.size();
    gpu_ctes_instancing->updateGPU(&ctes_instancing);

    is_dirty = false;
  }

  clearRenderDataInGPU();

  // Run the culling in the GPU
  comp_compute.sizes[0] = (uint32_t) objs.size();
  comp_compute.run(&comp_buffers);
}

// ---------------------------------------------------------------
// This is called when from the RenderManager
void CModuleGPUCulling::renderCategory(eRenderCategory category) {
  CGpuScope gpu_scope("GPU Culling");

  // Right now only supporting solids
  if (category != eRenderCategory::CATEGORY_SOLIDS)
    return;

  // Activate in the vs
  gpu_ctes_instancing->activate();

  // Offset to the args of the draw indexed instanced args in the draw_datas gpu buffer
  uint32_t offset = 0;
  uint32_t idx = 0;
  for( auto& render_type : render_types ) {
    CGpuScope gpu_render_type(render_type.title);

    // Because SV_InstanceID always start at zero, but the matrices
    // of each group have different starting offset
    ctes_instancing.instance_base = draw_datas[idx].base;
    gpu_ctes_instancing->updateGPU(&ctes_instancing);

    // Setup material & meshes
    render_type.material->activate();
    render_type.material->activateCompBuffers(&comp_buffers);
    render_type.mesh->activate();
    Render.ctx->DrawIndexedInstancedIndirect(gpu_draw_datas->buffer, offset);

    // The offset is in bytes
    offset += sizeof(TDrawData);
    ++idx;
  }

}

