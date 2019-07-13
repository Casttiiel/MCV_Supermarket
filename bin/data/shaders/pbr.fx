//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "common.fx"
#include "gbuffer.inc"
#include "pbr.inc"


float calculateNormalsEdge(
  float2 uv
) {

  float3 normal_center = txGNormal.Sample(samLinear, uv);

  float3 top = txGNormal.Sample(samLinear, uv + float2(0.0, CameraInvResolution.y));
  float3 topRight = txGNormal.Sample(samLinear, uv + CameraInvResolution);
  float3 right = txGNormal.Sample(samLinear, uv + float2(CameraInvResolution.x, 0.0));
  float3 bottomRight = txGNormal.Sample(samLinear, uv + float2(CameraInvResolution.x, -CameraInvResolution.y));
  float3 bottom = txGNormal.Sample(samLinear, uv - float2(0.0, CameraInvResolution.y));
  float3 bottomLeft = txGNormal.Sample(samLinear, uv - CameraInvResolution);
  float3 left = txGNormal.Sample(samLinear, uv - float2(CameraInvResolution.x, 0.0));
  float3 topLeft = txGNormal.Sample(samLinear, uv + float2(-CameraInvResolution.x, CameraInvResolution.y));

  float3 normal_sampledValue = top + topRight + right + bottom + bottomRight + left + bottomLeft + topLeft;

  normal_sampledValue /= 8.0f;

  return length(normal_center - normal_sampledValue);
}

float calculateDistanceEdge(
  float2 uv
) {

  float depth_center = txGLinearDepth.Sample(samLinear, uv).x;

  float top = txGLinearDepth.Sample(samLinear, uv + float2(0.0, CameraInvResolution.y)).x;
  float topRight = txGLinearDepth.Sample(samLinear, uv + CameraInvResolution).x;
  float right = txGLinearDepth.Sample(samLinear, uv + float2(CameraInvResolution.x, 0.0)).x;
  float bottomRight = txGLinearDepth.Sample(samLinear, uv + float2(CameraInvResolution.x, -CameraInvResolution.y)).x;
  float bottom = txGLinearDepth.Sample(samLinear, uv - float2(0.0, CameraInvResolution.y)).x;
  float bottomLeft = txGLinearDepth.Sample(samLinear, uv - CameraInvResolution).x;
  float left = txGLinearDepth.Sample(samLinear, uv - float2(CameraInvResolution.x, 0.0)).x;
  float topLeft = txGLinearDepth.Sample(samLinear, uv + float2(-CameraInvResolution.x, CameraInvResolution.y)).x; 

  float depth_sampledValue = top + topRight + right + bottom + bottomRight + left + bottomLeft + topLeft;

  depth_sampledValue /= 8.0f;

  return abs(depth_center - depth_sampledValue);
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
  VS_INPUT input
)
{
  // Use world from the constants uniform
  return runObjVS( input, World );
}

//--------------------------------------------------------------------------------------
// Vertex Shader for Skin, using standard vertex + skin info
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_skin(
  VS_INPUT input,
  VS_SKINNING skinning
)
{
  float4x4 SkinMatrix = getSkinMtx( skinning );
  return runObjVS( input, SkinMatrix );
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_instanced (
  VS_INPUT input,
  VS_INSTANCE_WORLD instance_data     // Stream 1
)
{
  // Use world from the instance
  float4x4 instanceWorld = getWorldOfInstance(instance_data);
  return runObjVS( input, instanceWorld );
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SHADER_CTE_BUFFER(TCtesMorph, CTE_BUFFER_SLOT_COMP_BUFFERS)
{
  float  morph_weight_target;
  float3 morph_pad3;
};

VS_OUTPUT VS_morph (
  VS_INPUT_MORPH input_morph
)
{
  float amount_of_next = morph_weight_target; //saturate( ( cos(GlobalWorldTime) + 1 ) * 0.5 );
  VS_INPUT input;
  input.Pos = input_morph.Pos * ( 1.0 - amount_of_next ) + input_morph.Pos1 * amount_of_next;
  input.N   = input_morph.N   * ( 1.0 - amount_of_next ) + input_morph.N1   * amount_of_next;
  input.Uv  = input_morph.Uv;
  input.T   = input_morph.T;
  return runObjVS( input, World );
}

//--------------------------------------------------------------------------------------
// Pixel Shader to fill the gBuffer
//--------------------------------------------------------------------------------------
void PS_common(
      VS_OUTPUT input
, out float4 o_albedo
, out float4 o_normal
, out float1 o_depth
, out float4 o_self_illum
, bool use_alpha_test
)
{
  float4 albedo_color = txAlbedo.Sample(samLinear, input.Uv);
  if( use_alpha_test && albedo_color.a <= 0.2 )
    clip(-1);

  o_albedo.xyz = albedo_color.xyz;
  o_albedo.a = txMetallic.Sample(samLinear, input.Uv).r;

  // Normal mapping
  float4 N_tangent_space = txNormal.Sample(samLinear, input.Uv);  // Between 0..1
  N_tangent_space.xyz = N_tangent_space.xyz * 2 - 1.;                           // Between -1..1

  float3x3 TBN = computeTBN( input.N, input.T );
  float3 N = mul( N_tangent_space.xyz, TBN );   // Normal from NormalMap
  N = normalize(N);

  // Save roughness in the alpha coord of the N render target
  float roughness = txRoughness.Sample(samLinear, input.Uv).r;
  o_normal = encodeNormal(N, roughness);

  float3 cam2obj = input.WorldPos.xyz - CameraPosition.xyz;
  float  linear_depth = dot( cam2obj, CameraFront ) / CameraZFar;

  o_depth = linear_depth;

  o_self_illum = txEmissive.Sample(samLinear,input.Uv);
  //o_self_illum.xyz *= ObjColor * self_illum_intensity;
}

void PS( VS_OUTPUT input
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth  : SV_Target2
, out float4 o_self_illum : SV_Target3
) {
  PS_common( input, o_albedo, o_normal, o_depth, o_self_illum, false );
}

void PS_alpha( VS_OUTPUT input
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth  : SV_Target2
, out float4 o_self_illum : SV_Target3
) {
  PS_common( input, o_albedo, o_normal, o_depth, o_self_illum, true );
}

//--------------------------------------------------------------------------------------
void PS_gbuffer_mix(
  VS_OUTPUT input
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth  : SV_Target2
) {
  // This is different -----------------------------------------
  float2 iTex0 = input.Uv * 2;
  float2 iTex1 = input.Uv;

  float4 weight_texture_boost = txMixBlendWeights.Sample(samLinear, iTex1); 

  float4 albedoR = txAlbedo.Sample(samLinear, iTex0);
  float4 albedoG = txAlbedo1.Sample(samLinear, iTex0);
  float4 albedoB = txAlbedo2.Sample(samLinear, iTex0);

  // Use the alpha of the albedo as heights + texture blending extra weights + material ctes extra weights (imgui)
  float w1, w2, w3;
  computeBlendWeights( albedoR.a + mix_boost_r + weight_texture_boost.r
                     , albedoG.a + mix_boost_g + weight_texture_boost.g
                     , albedoB.a + mix_boost_b + weight_texture_boost.b
                     , w1, w2, w3 );

  // Use the weight to 'blend' the albedo colors
  float4 albedo = albedoR * w1 + albedoG * w2 + albedoB * w3;
  o_albedo.xyz = albedo.xyz;

  // Mix the normal
  float3 normalR = txNormal.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
  float3 normalG = txNormal1.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
  float3 normalB = txNormal2.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
  float3 normal_color = normalR * w1 + normalG * w2 + normalB * w3; 
  float3x3 TBN = computeTBN( input.N, input.T );

  // Normal map comes in the range 0..1. Recover it in the range -1..1
  float3 wN = mul( normal_color, TBN );
  float3 N = normalize( wN );

  // Missing: Do the same with the metallic & roughness channels
  // ...

  // Possible plain blending without heights
  //o_albedo.xyz = lerp( albedoB.xyz, albedoG.xyz, weight_texture_boost.y );
  //o_albedo.xyz = lerp( o_albedo.xyz, albedoR.xyz, weight_texture_boost.x );

  //o_albedo.xyz = float3( iTex1.xy, 0 );   // Show the texture coords1

  //o_albedo.xyz = weight_texture_boost.xyz;  // Show the extra weight textures

  o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

  // This is the same -----------------------------------------
  // Save roughness in the alpha coord of the N render target
  float roughness = txRoughness.Sample(samLinear, iTex0).r;
  o_normal = encodeNormal( N, roughness );

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
  float3 cam2obj = input.WorldPos.xyz - CameraPosition.xyz;
  float  linear_depth = dot( cam2obj, CameraFront ) / CameraZFar;
  o_depth = linear_depth;
}


// -------------------------------------------------
// The output of the deferred before the postFX
float4 PS_GBuffer_Resolve( 
  float4 iPosition   : SV_POSITION,
  float2 iUV         : TEXCOORD0
  ) : SV_Target
{
  int3 ss_load_coords = uint3(iPosition.xy, 0);

  float4 acc_light = txAccLights.Load(ss_load_coords);
  float4 albedo_color = txGAlbedo.Load(ss_load_coords);
  float  linear_depth = txGLinearDepth.Sample(samLinear, iUV).x;
  
  //outlines
  float edgeN = calculateNormalsEdge(iUV);
  float edgeD = calculateDistanceEdge(iUV);

  //return lerp(acc_light, float4(0,0,0,1), step(0.3, (edgeN + edgeD)));
  return acc_light - (edgeD + edgeN) * acc_light;
}

// -------------------------------------------------
// Gloss = 1 - rough*rough
float3 Specular_F_Roughness(float3 specularColor, float gloss, float3 h, float3 v) {
  // Sclick using roughness to attenuate fresnel.
  return (specularColor + (max(gloss, specularColor) - specularColor) * pow((1 - saturate(dot(v, h))), 5));
}

float NormalDistribution_GGX(float a, float NdH)
{
    // Isotropic ggx.
    float a2 = a*a;
    float NdH2 = NdH * NdH;

    float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
    denominator *= denominator;
    denominator *= PI;

    return a2 / denominator;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
        // Smith schlick-GGX.
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);
    return GV * GL;
}

float Specular_D(float a, float NdH)
{
    return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) 
{
    return Geometric_Smith_Schlick_GGX( a, NdV, NdL );
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular_F(float3 specularColor, float3 h, float3 v)
{
    return Fresnel_Schlick(specularColor, h, v);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)
{
    return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h) ) / (4.0f * NdL * NdV + 0.0001f);
}

// -------------------------------------------------
float4 PS_Ambient(
  in float4 iPosition : SV_Position
, in float2 iUV : TEXCOORD0
) : SV_Target
{

  // Declare some float3 to store the values from the GBuffer
  GBuffer g;
  decodeGBuffer( iPosition.xy, g );

  // if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
  // if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
  float mipIndex = g.roughness * g.roughness * 8.0f;
  float3 env = txEnvironmentMap.SampleLevel(samLinear, g.reflected_dir, mipIndex).xyz;
  // Convert the color to linear also.
  env = pow(abs(env), 2.2f);
  //return float4( env, 1 );

  // The irrandiance, is read using the N direction.
  // Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
  // and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
  // Remove the interpolation in the final version!!!
  float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, g.N, 4).xyz;
  float3 irradiance = irradiance_mipmaps;
  float3 irradiance_texture = txIrradianceMap.Sample(samLinear, g.N).xyz;
  irradiance = irradiance_texture;
  //return float4( irradiance_texture, 1 );
  //float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * ( 1. - scalar_irradiance_vs_mipmaps );
  //return float4( irradiance, 1 );

  // How much the environment we see
  float3 env_fresnel = Specular_F_Roughness(g.specular_color, 1. - g.roughness * g.roughness, g.N, g.view_dir);
  //return float4(env_fresnel, 1 );

  float ao = txAO.Sample( samLinear, iUV).x;

  float g_ReflectionIntensity = 1.0;
  float g_AmbientLightIntensity = 1.0;

  float4 final_color = (float4(env_fresnel * env * g_ReflectionIntensity + 
                              g.albedo.xyz * irradiance * g_AmbientLightIntensity
                              , 1.0f) * GlobalAmbientBoost);
  final_color.xyz += g.self_illum;

  return final_color * ao;
}

// ----------------------------------------
// Passthrough for the lights geometry
void VS_pass(
  in float4 iPos : POSITION
, out float4 oPos : SV_POSITION
) {
  float4 world_pos = mul(iPos, World);
  oPos = mul(world_pos, ViewProjection);
}

// ----------------------------------------
void VS_skybox( 
  in float4 iPosition  : POSITION 
, in float4 iColor     : COLOR0 
, out float4 oPosition : SV_Position 
) {
  // Convert the range 0..1 from iPosition to -1..1 to match the homo space
  oPosition = float4(iPosition.x * 2 - 1., 1 - iPosition.y * 2, 1, 1);
}

// --------------------------------------------------------
float4 PS_skybox( in float4 iPosition : SV_Position ) : SV_Target
{
  float3 view_dir = mul( float4( iPosition.xy, 1, 1 ), CameraScreenToWorld ).xyz;
  float4 skybox_color = txEnvironmentMap.Sample(samLinear, view_dir);
  return float4(skybox_color.xyz,1) * GlobalAmbientBoost;
}

// --------------------------------------------------------
float3 Diffuse(float3 pAlbedo) {
    return pAlbedo/PI;
}

// --------------------------------------------------------
float4 shade( float4 iPosition, bool use_shadows, bool fix_shadows ) {

  // Declare some float3 to store the values from the GBuffer
  GBuffer g;
  decodeGBuffer( iPosition.xy, g );

  // Shadow factor entre 0 (totalmente en sombra) y 1 (no ocluido)
  float shadow_factor = use_shadows ? getShadowFactor( g.wPos ) : 1.;

  if(fix_shadows){
    float4 worldPos = float4(g.wPos,1);
    float4 almostScreenPos = mul(worldPos, LightViewProjOffset);
    float3 screenPos = almostScreenPos.xyz / almostScreenPos.w;
    //if out of range, shadow_factor = 0;
    if(screenPos.x < -1 || screenPos.x > 1 || screenPos.y < -1 || screenPos.y > 1)
      shadow_factor = 0;
  }

  // From wPos to Light
  float3 light_dir_full = LightPosition.xyz - g.wPos;
  float  distance_to_light = length( light_dir_full );
  float3 light_dir = light_dir_full / distance_to_light;

  float  NdL = saturate(dot(g.N, light_dir));
  float  NdV = saturate(dot(g.N, g.view_dir));
  float3 h   = normalize(light_dir + g.view_dir); // half vector
  
  float  NdH = saturate(dot(g.N, h));
  float  VdH = saturate(dot(g.view_dir, h));
  float  LdV = saturate(dot(light_dir, g.view_dir));
  float  a   = max(0.001f, g.roughness * g.roughness);
  float3 cDiff = Diffuse(g.albedo);
  float3 cSpec = Specular(g.specular_color, h, g.view_dir, light_dir, a, NdL, NdV, NdH, VdH, LdV);

  //float3 lut = txLut.Sample(samLinear, float2(NdL, 0)).xyz;

  float att = saturate( distance_to_light / LightRadius );
  att = 1. - att;
  float3 final_color = LightColor.xyz * NdL * (cDiff * (1.0f - cSpec) + cSpec) * att * LightIntensity * shadow_factor;
  //float3 final_color = LightColor.xyz * lut * (cDiff * (1.0f - cSpec) + cSpec) * att * LightIntensity * shadow_factor * ObjColor;
  return float4( final_color, 1);
}

// -------------------------------------------------
float4 PS_point_lights(
  in float4 iPosition : SV_Position
) : SV_Target
{
  return shade( iPosition, false, false );
}

float4 PS_dir_lights(
  in float4 iPosition : SV_Position
) : SV_Target
{
  return shade( iPosition, true, false );
}

float4 PS_dir_lights_no_shadow(
  in float4 iPosition : SV_Position
) : SV_Target
{
  return shade( iPosition, false, true );
}

// -------------------------------------------------
void VS_fake_volumetric_lights(
  in float4 iPos : POSITION
, out float4 oPos : SV_POSITION
, out float4 world_pos : TEXCOORD0
) {
  world_pos = mul(iPos, World);
  oPos = mul(world_pos, ViewProjection);
}


float4 PS_fake_volumetric_lights(
  in float4 iPosition : SV_Position
, in float4 world_pos : TEXCOORD0
) : SV_Target
{

  world_pos.xyz /= world_pos.w;

  float max_distance = 10.0 + (LightIntensity * 0.3) - GlobalAmbientBoost * 5.0;

  float3 light_dir_full = world_pos.xyz - LightPosition.xyz;
  float  d = dot( light_dir_full, LightFront );

  float att = saturate( d / max_distance );

  float alpha = (1 - att);

  //when distance is closer alpha goes to 0 
  float d2 = CameraPosition - world_pos.xyz;
  float dist_att = d2 / (max_distance * 5);
  dist_att = saturate(dist_att);

  alpha *= dist_att;

  return float4(LightColor.xyz,alpha);
}