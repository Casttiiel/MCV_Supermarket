#include "common.fx"

//--------------------------------------------------------------------------------------
// This shader is expected to be used only with the mesh unitQuadXY.mesh
// Where the iPos goes from 0,0..1,1
void VS(
    in float4 iPos : POSITION
  , in float2 iColor : COLOR0
  , out float4 oPos : SV_POSITION
  , out float2 oTex0 : TEXCOORD0
  , out float4 oTex1 : TEXCOORD1
  , out float4 oTex2 : TEXCOORD2
  , out float4 oTex3 : TEXCOORD3
)
{
  /*// Passthrough of coords and UV's
  oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
  oTex0 = iPos.xy;
  // Save in the xy the positive offset 
  // Save in the zw the negative offset 
  oTex1.xy = oTex0 + blur_step * blur_d.x;
  oTex1.zw = oTex0 - blur_step * blur_d.x;
  oTex2.xy = oTex0 + blur_step * blur_d.y;
  oTex2.zw = oTex0 - blur_step * blur_d.y;
  oTex3.xy = oTex0 + blur_step * blur_d.z;
  oTex3.zw = oTex0 - blur_step * blur_d.z;
*/

  // Passthrough of coords and UV's
  oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
  oTex0 = iPos.xy;

  float2 dir = normalize(oTex0 - float2(0.5f,0.5f));
  float strength = blur_step;
  float radial_strength = distance(oTex0, float2(0.5f,0.5f));
  //radial_strength = radial_strength < 1.0 ? 0.0f : radial_strength;
  
  // Save in the xy the positive offset 
  // Save in the zw the negative offset 
  oTex1.xy = oTex0 - dir *  radial_strength * strength * blur_d.x;
  oTex1.zw = oTex0 - dir * 1.5f *  radial_strength * strength * blur_d.x;
  oTex2.xy = oTex0 - dir *  radial_strength * strength * blur_d.y;
  oTex2.zw = oTex0 - dir * 1.5f *  radial_strength * strength * blur_d.y;
  oTex3.xy = oTex0 - dir *  radial_strength * strength * blur_d.z;
  oTex3.zw = oTex0 - dir * 1.5f *  radial_strength * strength * blur_d.z;

}

//--------------------------------------------------------------------------------------
float4 PS(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
  , in float4 iTex1 : TEXCOORD1
  , in float4 iTex2 : TEXCOORD2
  , in float4 iTex3 : TEXCOORD3
) : SV_Target
{

  // 7 tap blur controlled by the vs
  float4 cp3 = txAlbedo.Sample(samClampLinear, iTex3.zw);
  float4 cp2 = txAlbedo.Sample(samClampLinear, iTex2.zw);
  float4 cp1 = txAlbedo.Sample(samClampLinear, iTex1.zw);
  float4 c0  = txAlbedo.Sample(samClampLinear, iTex0.xy);
  float4 cn1 = txAlbedo.Sample(samClampLinear, iTex1.xy);
  float4 cn2 = txAlbedo.Sample(samClampLinear, iTex2.xy);
  float4 cn3 = txAlbedo.Sample(samClampLinear, iTex3.xy);

  float4 cfinal =
      (c0       ) * blur_w.x
    + (cp1 + cn1) * blur_w.y
    + (cp2 + cn2) * blur_w.z
    + (cp3 + cn3) * blur_w.w
    ;

  return cfinal;
}
