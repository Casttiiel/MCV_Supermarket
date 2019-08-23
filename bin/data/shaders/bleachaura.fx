//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "common.fx"


float4 getNewVertPosition( float4 p, float3 N )
{
  float freqx = 1.0f + sin(GlobalWorldTime) * 1.0f;
  float freqy = 1.0f + sin(GlobalWorldTime * 1.3f) * 1.0f;
  float freqz = 1.0f + sin(GlobalWorldTime * 1.1f) * 1.0f;
  float amp = sin(GlobalWorldTime * 1.4) * 0.3f;
  
  float f = sin(N.x * freqx + GlobalWorldTime) * sin(N.y * freqy + GlobalWorldTime) * sin(N.z * freqz + GlobalWorldTime);
  p.z += N.z * amp * f;
  p.x += N.x * amp * f;
  p.y += N.y * amp * f;
  return p;
}


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
  float4 newPos = getNewVertPosition(Pos, N);
  output.Pos = mul(newPos, World);
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
  float4 red = float4(1,0,0,1);
  float4 yellow = float4(1,1,0,1);
  float4 orange = float4(1,0.2f,0,1);
  float4 blue = float4(0,0,1,1);
  float4 green = float4(0,1,0,1);

  //for distortion
  const float _scrollX = 0.0f;
  const float _scrollY = 0.6f;
  //for visuals
  const float _threshold = 0.75f;
  const float _scale = 4.0f;

  float4 form = txAlbedo.Sample(samLinear,  float2((input.Uv.x + GlobalWorldTime * _scrollX)  ,(input.Uv.y - GlobalWorldTime * _scrollY)) * float2(_scale,_scale));
  float4 voronoi_noise = txNormal.Sample(samLinear, float2((input.Uv.x + GlobalWorldTime * _scrollX)  ,(input.Uv.y - GlobalWorldTime * _scrollY)) * float2(_scale*3,_scale));
  voronoi_noise.a = voronoi_noise.z;
  float shapetex = lerp(3, -1.0,input.Uv.y);
  
  voronoi_noise += shapetex * _threshold * 0.9f;
  float4 final_voronoi = voronoi_noise;

  float flame = final_voronoi.x > _threshold;
  float4 flamecolored = flame * ObjColor * form;

  return flamecolored;
}

