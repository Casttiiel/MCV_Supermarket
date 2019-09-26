#include "common.fx"

#define FXAA_PC 1
#define FXAA_HLSL_4 1
#define FXAA_QUALITY__PRESET 39
#define FXAA_GREEN_AS_LUMA 1

#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define ITERATIONS 12
#define offset 0.5
#define SUBPIXEL_QUALITY 0.75


#include "Fxaa3_11.h"

//--------------------------------------------------------------------------------------
// This shader is expected to be used only with the mesh unitQuadXY.mesh
// Where the iPos goes from 0,0..1,1
void VS(
    in float4 iPos : POSITION
  , out float4 oPos : SV_POSITION
  , out float2 oTex0 : TEXCOORD0
)
{
  // Passthrough of coords and UV's
  oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
  oTex0 = iPos.xy;
}

float rgb2luma(float3 rgb){
    return sqrt(dot(rgb, float3(0.299, 0.587, 0.114)));
}

float QUALITY(float i){
  float arr[12] = {1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0,8.0,8.0,8.0,8.0,8.0};
  return arr[i];
}

//--------------------------------------------------------------------------------------
float4 PS(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{
  //ONE VERSION
  FxaaTex inputFXAATex = { samClampBiLinear, txAlbedo };
  float pixelWidth = (1.0f / screenWidth);
  float pixelHeight = (1.0f / screenHeight);

  float2 pixelCenter = float2(iTex0.x - pixelWidth, iTex0.y - pixelHeight);

  return FxaaPixelShader(
    pixelCenter,                             // FxaaFloat2 pos,
    FxaaFloat4(0,0,0,0),                     // FxaaFloat4 fxaaConsolePosPos,
    inputFXAATex,                            // FxaaTex tex,
    inputFXAATex,                            // FxaaTex fxaaConsole360TexExpBiasNegOne,
    inputFXAATex,                            // FxaaTex fxaaConsole360TexExpBiasNegTwo,
    float2(pixelWidth,pixelHeight),          // FxaaFloat2 fxaaQualityRcpFrame,
    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),      // FxaaFloat4 fxaaConsoleRcpFrameOpt,
    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),      // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),      // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
    sub_pix,                                 // FxaaFloat fxaaQualitySubpix,
    edge_threshold,                          // FxaaFloat fxaaQualityEdgeThreshold,
    edge_threshold_min,                      // FxaaFloat fxaaQualityEdgeThresholdMin,
    0.f,                                     // FxaaFloat fxaaConsoleEdgeSharpness,
    0.f,                                     // FxaaFloat fxaaConsoleEdgeThreshold,
    0.f,                                     // FxaaFloat fxaaConsoleEdgeThresholdMin,
    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f)       // FxaaFloat fxaaConsole360ConstDir,
  );
}

//--------------------------------------------------------------------------------------
float4 PS_alt(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{
  float pixelWidth = (1.0f / screenWidth);
  float pixelHeight = (1.0f / screenHeight);

  float2 pixelCenter = float2(iTex0.x - pixelWidth, iTex0.y - pixelHeight);

  float3 colorCenter = txAlbedo.Sample(samClampBiLinear, iTex0).xyz;

  // Luma at the current fragment
  float lumaCenter = rgb2luma(colorCenter);

  // Luma at the four direct neighbours of the current fragment.
  float lumaDown = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(0,-pixelHeight)).xyz);
  float lumaUp = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(0,pixelHeight)).xyz);
  float lumaLeft = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(-pixelWidth,0)).xyz);
  float lumaRight = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(pixelWidth,0)).xyz);
  
  // Find the maximum and minimum luma around the current fragment.
  float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
  float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

  // Compute the delta.
  float lumaRange = lumaMax - lumaMin;

  // If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA.
  if(lumaRange < max(EDGE_THRESHOLD_MIN,lumaMax*EDGE_THRESHOLD_MAX)){
    return float4(colorCenter,1);
  }
  
  // Query the 4 remaining corners lumas.
  float lumaDownLeft = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(-pixelWidth,-pixelHeight)).xyz);
  float lumaUpRight = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(pixelWidth,pixelHeight)).xyz);
  float lumaUpLeft = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(-pixelWidth,pixelHeight)).xyz);
  float lumaDownRight = rgb2luma(txAlbedo.Sample(samClampBiLinear, iTex0 + float2(pixelWidth,-pixelHeight)).xyz);

  // Combine the four edges lumas (using intermediary variables for future computations with the same values).
  float lumaDownUp = lumaDown + lumaUp;
  float lumaLeftRight = lumaLeft + lumaRight;

  // Same for corners
  float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
  float lumaDownCorners = lumaDownLeft + lumaDownRight;
  float lumaRightCorners = lumaDownRight + lumaUpRight;
  float lumaUpCorners = lumaUpRight + lumaUpLeft;

  // Compute an estimation of the gradient along the horizontal and vertical axis.
  float edgeHorizontal =  abs(-2.0 * lumaLeft + lumaLeftCorners)  + abs(-2.0 * lumaCenter + lumaDownUp ) * 2.0    + abs(-2.0 * lumaRight + lumaRightCorners);
  float edgeVertical =    abs(-2.0 * lumaUp + lumaUpCorners)      + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0  + abs(-2.0 * lumaDown + lumaDownCorners);

  // Is the local edge horizontal or vertical ?
  bool isHorizontal = (edgeHorizontal >= edgeVertical);

  // Select the two neighboring texels lumas in the opposite direction to the local edge.
  float luma1 = isHorizontal ? lumaDown : lumaLeft;
  float luma2 = isHorizontal ? lumaUp : lumaRight;
  // Compute gradients in this direction.
  float gradient1 = luma1 - lumaCenter;
  float gradient2 = luma2 - lumaCenter;

  // Which direction is the steepest ?
  bool is1Steepest = abs(gradient1) >= abs(gradient2);

  // Gradient in the corresponding direction, normalized.
  float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

  // Choose the step size (one pixel) according to the edge direction.
  float stepLength = isHorizontal ? CameraInvResolution.y : CameraInvResolution.x;

  // Average luma in the correct direction.
  float lumaLocalAverage = 0.0;

  if(is1Steepest){
      // Switch the direction
      stepLength = - stepLength;
      lumaLocalAverage = 0.5*(luma1 + lumaCenter);
  } else {
      lumaLocalAverage = 0.5*(luma2 + lumaCenter);
  }

  // Shift UV in the correct direction by half a pixel.
  float2 currentUv = iTex0;
  if(isHorizontal){
      currentUv.y += stepLength * 0.5;
  } else {
      currentUv.x += stepLength * 0.5;
  }
  
  // If both sides have not been reached, continue to explore.
  bool reachedBoth = false;
  bool reached1 = false;
  bool reached2 = false;
  float lumaEnd1 = 0;
  float lumaEnd2 = 0;
  float2 uv1 = currentUv;
  float2 uv2 = currentUv;
  if(!reachedBoth){
    for(int i = 2; i < ITERATIONS; i++){
        // If needed, read luma in 1st direction, compute delta.
        if(!reached1){
            lumaEnd1 = rgb2luma(txAlbedo.Sample(samClampBiLinear, uv1).xyz);
            lumaEnd1 = lumaEnd1 - lumaLocalAverage;
        }
        // If needed, read luma in opposite direction, compute delta.
        if(!reached2){
            lumaEnd2 = rgb2luma(txAlbedo.Sample(samClampBiLinear, uv2).xyz);
            lumaEnd2 = lumaEnd2 - lumaLocalAverage;
        }
        // If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
        reached1 = abs(lumaEnd1) >= gradientScaled;
        reached2 = abs(lumaEnd2) >= gradientScaled;
        reachedBoth = reached1 && reached2;

        // If the side is not reached, we continue to explore in this direction, with a variable quality.
        if(!reached1){
            uv1 -= offset * QUALITY(i);
        }
        if(!reached2){
            uv2 += offset * QUALITY(i);
        }

        // If both sides have been reached, stop the exploration.
        if(reachedBoth){ break;}
    }
  }

  // Compute the distances to each extremity of the edge.
  float distance1 = isHorizontal ? (iTex0.x - uv1.x) : (iTex0.y - uv1.y);
  float distance2 = isHorizontal ? (uv2.x - iTex0.x) : (uv2.y - iTex0.y);

  // In which direction is the extremity of the edge closer ?
  bool isDirection1 = distance1 < distance2;
  float distanceFinal = min(distance1, distance2);

  // Length of the edge.
  float edgeThickness = (distance1 + distance2);

  // UV offset: read in the direction of the closest side of the edge.
  float pixelOffset = - distanceFinal / edgeThickness + 0.5;


  // Is the luma at center smaller than the local average ?
  bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

  // If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
  // (in the direction of the closer side of the edge.)
  bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

  // If the luma variation is incorrect, do not offset.
  float finalOffset = correctVariation ? pixelOffset : 0.0;

  // Sub-pixel shifting
  // Full weighted average of the luma over the 3x3 neighborhood.
  float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
  // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
  float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaRange,0.0,1.0);
  float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
  // Compute a sub-pixel offset based on this delta.
  float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

  // Pick the biggest of the two offsets.
  finalOffset = max(finalOffset,subPixelOffsetFinal);

  // Compute the final UV coordinates.
  float2 finalUv = iTex0;
  if(isHorizontal){
      finalUv.y += finalOffset * stepLength;
  } else {
      finalUv.x += finalOffset * stepLength;
  }

  // Read the color at the new UV coordinates, and use it.
  float4 finalColor = txAlbedo.Sample(samClampBiLinear,finalUv);
  return finalColor;
}
  