//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos      : SV_POSITION;
  float3 N        : NORMAL;
  float2 Uv       : TEXCOORD0;
  float3 WorldPos : TEXCOORD1;
  float4 T        : NORMAL1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
  float4 Pos : POSITION,
  float3 N : NORMAL,
  float2 Uv: TEXCOORD0,
  float4 T : NORMAL1
)
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(Pos, World);
  output.WorldPos = output.Pos.xyz;
  output.Pos = mul(output.Pos, ViewProjection);
  output.N = mul(N, (float3x3)World);
  output.T = float4( mul(T.xyz, (float3x3)World), T.w);
  output.Uv = Uv;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS(
      VS_OUTPUT input
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth  : SV_Target2
)
{
  float4 noise0 = txNoise.Sample(samLinear,input.Uv * 0.05 + 0.0008 * GlobalWorldTime * float2(0,1) ) * 10. - 5;
  float a = sin(noise0.x);
  float b = cos(noise0.y);
  float4 albedo_color = txAlbedo.Sample(samLinear, input.Uv * 0.5 + float2(a,b));
  o_albedo.xyz = albedo_color * ObjColor;
  o_albedo.a = 0;

  // Normal mapping
  float4 N_tangent_space = txNormal.Sample(samLinear, input.Uv);  // Between 0..1
  N_tangent_space.xyz = N_tangent_space.xyz * 2 - 1.;                           // Between -1..1

  float3 T = input.T.xyz;
  float3 B = cross(T, input.N) * input.T.w;
  float3x3 TBN = float3x3(T,B,input.N);
  float3 N = mul( N_tangent_space.xyz, TBN );   // Normal from NormalMap

  // Save roughness in the alpha coord of the N render target
  float roughness = 0;
  o_normal = encodeNormal(N, roughness);

  float3 cam2obj = input.WorldPos.xyz - CameraPosition.xyz;
  float  linear_depth = dot( cam2obj, CameraFront ) / CameraZFar;

  o_depth = linear_depth;
}
