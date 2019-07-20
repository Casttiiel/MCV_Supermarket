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
  const float fire_amount = 2.0f; //touch this for fire amount
  const float fire_speed = 1.0f; //touch this for fire speed
  const float fire_clamp = 0.3f; //touch this to make fire disappear more or less
  const float fire_clamp_attenuation_range = 0.1f; //touch this to make fire start disappear totally
  const float fire_clamp_range = 0.1f; //touch this to make fire start disappear totally
  const float fire_attenuation_range = 0.2f; //touch this to make fire start disappear earlier
  float2 uv = float2(input.Uv.x * fire_amount, input.Uv.y + GlobalWorldTime * fire_speed);

  float tex1 = txNormal.Sample(samLinear, uv).x; //use this for the color ramp
  float tex2 = txMetallic.Sample(samLinear, uv + float2(0,GlobalWorldTime * 0.5f)).x;
  //float3 tex2 = txRoughness.Sample(samLinear, uv).xyz;

  float3 color_ramp = txAlbedo.Sample(samLinear, float2(1-tex1,0.5f)).xyz;

  float alpha;
  if(input.Uv.y <= fire_clamp_attenuation_range){
    alpha = ((tex1 * tex2) > fire_clamp) * (input.Uv.y) / fire_clamp_attenuation_range;
  }else if(input.Uv.y <= fire_clamp_attenuation_range + fire_clamp_range){
    alpha = (tex1 * tex2) > fire_clamp;
  }else if(input.Uv.y <= fire_clamp_attenuation_range + fire_clamp_range + fire_attenuation_range){
    alpha = ((tex1 * tex2) > fire_clamp) ? 1.0 : (input.Uv.y - fire_clamp_range - fire_clamp_attenuation_range) / fire_attenuation_range;
  }else{
    alpha = 1.0f;
  }
 
  return float4(color_ramp, alpha);
}

float4 PS_aux(VS_OUTPUT input) : SV_Target
{
  const float lineAmplitude = 1.0f;
  const float lineLength = 20.0f;
  const float lineSpeed = 1.0f;

  float2 uv = float2(input.Uv.x * lineLength, (input.Uv.y * lineAmplitude) + (GlobalWorldTime * lineSpeed));
  float2 uv2 = uv + float2( 1, 1) + float2(sin(GlobalWorldTime), cos(GlobalWorldTime));

  float3 tex1 = txNormal.Sample(samLinear, uv); //use this for the color ramp
  float3 tex2 = txMetallic.Sample(samLinear, uv);
  float3 tex3 = txMetallic.Sample(samLinear, uv2);

  float3 tex4 = (tex2 * tex3);

  return float4(tex2, 1);
}