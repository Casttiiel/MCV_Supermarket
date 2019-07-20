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
  float4 T        : NORMAL1;
};


float4 getNewVertPosition( float4 p, float3 N )
{
  float freqx = 1.0f + sin(GlobalWorldTime) * 1.0f;
  float freqy = 1.0f + sin(GlobalWorldTime * 1.3f) * 1.0f;
  float freqz = 1.0f + sin(GlobalWorldTime * 1.1f) * 1.0f;
  float amp = sin(GlobalWorldTime * 1.4);
  
  float f = sin(N.x * freqx + GlobalWorldTime) * sin(N.y * freqy + GlobalWorldTime) * sin(N.z * freqz + GlobalWorldTime);
  p.z += N.z * amp * f;
  p.x += N.x * amp * f;
  p.y += N.y * amp * f;
  return p;
}

// 2D Random
float random (in float2 st) {
    return frac(sin(dot(st.xy,
                         float2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in float2 st) {
    float2 i = floor(st);
    float2 f = frac(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + float2(1.0, 0.0));
    float c = random(i + float2(0.0, 1.0));
    float d = random(i + float2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    float2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return lerp(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}


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

  float4 bitangent = float4(cross(N, T.xyz),1.0f);
  float4 positionAndTangent = getNewVertPosition( Pos + T * 0.01, N );
  float4 positionAndBitangent = getNewVertPosition( Pos + bitangent * 0.01, N );
  float4 newTangent = ( positionAndTangent - Pos ); // leaves just 'tangent'
  newTangent = normalize(newTangent);
  float4 newBitangent = ( positionAndBitangent - Pos ); // leaves just 'bitangent'
  newBitangent = normalize(newBitangent);
  float3 newNormal = cross( newTangent.xyz, newBitangent.xyz );
  newNormal = normalize(newNormal);

  output.Pos = mul(Pos, World);
  output.Pos = mul(output.Pos, ViewProjection);
  output.N = mul(N, (float3x3)World);
  output.T = float4( mul(T.xyz, (float3x3)World), T.w);
  output.Uv = Uv;
  return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
  const float freq = 15.0f;
  const float speed = 5.0f;
  const float amplitude = 22.0f;

  float2 pos = float2(input.Uv * freq);

  // Use the noise function
  float3 dir = normalize(-CameraFront.xyz);
  float3 N = normalize(input.N);
  float  NdV = saturate(dot(N, dir));

  float npatron = noise(pos);

  npatron = sin(2.*sin(npatron*amplitude + GlobalWorldTime*speed)) * 0.5 + 0.5;
  npatron *= npatron;
  npatron *= npatron;
  npatron *= npatron;
  npatron *= npatron;



  float3 iridiscolor = float3(0,0,0);
  iridiscolor += sin(NdV * float3(0.0, 1.0, 0.0) * 10.0 * 1.5);
  iridiscolor += sin(NdV * float3(1.0, 0.0, 0.0) * 20.0 * 1.5);
  iridiscolor += sin(NdV * float3(0.0, 0.0, 1.0) * 5.0 * 1.5);
  iridiscolor = clamp(normalize(iridiscolor), 0.0, 1.0);
  //iridiscolor = float3(1,1,1);
  //iridiscolor = float3(0,0,0);

  //npatron *= ObjColor;
  float3 color = (ObjColor.xyz * (npatron <= 0.9f)) + (iridiscolor * (npatron > 0.9f));

  return float4(iridiscolor, 1.0f);
}