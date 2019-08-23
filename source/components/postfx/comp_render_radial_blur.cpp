#include "mcv_platform.h"
#include "comp_render_radial_blur.h"
#include "resources/resource.h"
#include "render/radial_blur_step.h"

DECL_OBJ_MANAGER("render_radial_blur", TCompRenderRadialBlur);

// ---------------------
void TCompRenderRadialBlur::debugInMenu() {
  ImGui::Checkbox("Enabled", &enabled);
  ImGui::DragInt("# Steps", &nactive_steps, 0.1f, 0, (int)steps.size());
  ImGui::InputFloat("Weights Center", &weights.x);
  ImGui::InputFloat("Weights 1st", &weights.y);
  ImGui::InputFloat("Weights 2nd", &weights.z);
  ImGui::InputFloat("Weights 3rd", &weights.w);
  if (ImGui::SmallButton("box")) {
    distance_factors = VEC4(1, 2, 3, 4);
    weights = VEC4(1, 1, 1, 1);
  }
  ImGui::SameLine();
  if (ImGui::SmallButton("gauss")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
  }
  ImGui::SameLine();
  if (ImGui::SmallButton("linear")) {
    // This is a 5 taps kernel (center + 2 taps on each side)
    // http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
    weights = VEC4(0.2270270270f, 0.3162162162f, 0.0702702703f, 0.f);
    distance_factors = VEC4(1.3846153846f, 3.2307692308f, 0.f, 0.f);
  }
  if (ImGui::SmallButton("Preset1")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
    global_distance = 2.7f;
    //nactive_steps = 3;
  }
  if (ImGui::SmallButton("Preset2")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
    global_distance = 2.0f;
    //nactive_steps = 2;
  }
  ImGui::DragFloat("global_distance", &global_distance, 0.01f, 0.1f, 32.0f);
  ImGui::InputFloat("Distance 2nd Tap", &distance_factors.x);
  ImGui::InputFloat("Distance 3rd Tap", &distance_factors.y);
  ImGui::InputFloat("Distance 4th Tap", &distance_factors.z);
  if (ImGui::SmallButton("Default Distances")) {
    distance_factors = VEC4(1, 2, 3, 4);
  }
}

void TCompRenderRadialBlur::load(const json& j, TEntityParseContext& ctx) {
  enabled = j.value( "enabled", true);
  global_distance = j.value("global_distance", 1.0f);
  distance_factors = VEC4(1, 2, 3, 4);

  weights.x = j.value("w0", 1.f);
  weights.y = j.value("w1", 1.f);
  weights.z = j.value("w2", 1.f);
  weights.w = j.value("w3", 1.f);
  
  if (j.value("box_filter", false))
    weights = VEC4(1, 1, 1, 1);
  else if (j.value("gauss_filter", false))
    weights = VEC4(70, 56, 28, 8);
    /*
                  1
                1   1
              1   2   1
            1   3   3   1
          1   4   6   4   1
        1   5   10 10   5   1
      1   6   15  20  15  6   1
    1   7   21  35  35  21  7   1
  1   8   28  56  70  56  28  8   1   <-- Four taps, discard the last 1
  */

  bool is_ok = true;
  int nsteps = j.value("max_steps", 2);
  int xres = Render.width;
  int yres = Render.height;

  std::string rt_name = j.value("rt_name", "Radial_Blur");

  // To generate unique names
  static int g_blur_counter = 0;
  for (int i = 0; i < nsteps; ++i) {
    CRadialBlurStep* s = new CRadialBlurStep;

    char blur_name[64];
    sprintf( blur_name, "%s_%02d", rt_name.c_str(), g_blur_counter);
    g_blur_counter++;

    is_ok &= s->create(blur_name, xres, yres, DXGI_FORMAT_R16G16B16A16_FLOAT);
    assert(is_ok);
    steps.push_back(s);
    xres /= 1.5;
    yres /= 1.5;
  }

  nactive_steps = (int)j.value("active_steps", steps.size());
}

CTexture* TCompRenderRadialBlur::apply( CTexture* in_texture) {
  if (!enabled)
    return in_texture;
  CGpuScope gpu_scope("CompRadialBlur");

  CTexture* output = in_texture;
  int nsteps_to_apply = nactive_steps;
  for (auto s : steps) {
    if (--nsteps_to_apply < 0)
      break;
    output = s->apply(in_texture, global_distance, distance_factors, weights);
    in_texture = output;
  }

  return output;
}

void TCompRenderRadialBlur::update(float delta) {
  if (!enabled)
    return;
  ratio = clamp(ratio + (delta / time), 0.0f, 1.0f);


  if (ratio == 1.0f)
    enabled = false;
}

void TCompRenderRadialBlur::enable(float t) {
  ratio = 0.0f;
  time = t;
  enabled = true;
}