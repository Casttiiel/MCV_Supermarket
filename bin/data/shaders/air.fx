//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "common.fx"

SHADER_CTE_BUFFER(TCtesAir, CTE_BUFFER_SLOT_COMP_BUFFERS)
{
  float  height;
  float  v0;
  float  pitch;
  float  length;
};

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
  float4 auxPos = Pos;
  output.Pos = mul(auxPos, World);
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
float4 PS(VS_OUTPUT input) : SV_Target
{
  const float l = 0.5f;
  const float d = 0.2f;
  const float t = 0.0f;

  float alpha_horizontal = input.Uv.y >= 0.5f ? 1 - input.Uv.y : input.Uv.y;
  float alpha_vertical = 1.0f;
  if(input.Uv.x >= (t + (l/2.0f))){
    alpha_vertical = (input.Uv.x - (t + (l/2.0f)) / d);
  }else if(input.Uv.x <= (t - (l/2.0f))){
    alpha_vertical = 0.0f;
  }
 
  return float4(1,1,1, alpha_horizontal * alpha_vertical);
}