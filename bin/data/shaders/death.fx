//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "common.fx"

float noise( in float2 x )
{
    x.x +=0.3*cos(x.y+(GlobalWorldTime*0.3));//crawling under myyyy skinnn!
    x.y +=0.3*sin(x.x);
    float2 p = floor(x);
    float2 f = frac(x);
    f = f*f*(3.0-2.0*f);
 
    //edit: using texture instead of textureLod so this compiles+runs on mobile
    float4 a_vec = txAlbedo.Sample(samLinear,(p+float2(0.5,0.5))/256.0,0);
	float4 b_vec = txAlbedo.Sample(samLinear,(p+float2(1.5,0.5))/256.0,0);
	float4 c_vec = txAlbedo.Sample(samLinear,(p+float2(0.5,1.5))/256.0,0);
	float4 d_vec = txAlbedo.Sample(samLinear,(p+float2(1.5,1.5))/256.0,0);   
   
    float a = a_vec.x;
    float b = b_vec.x;
    float c = c_vec.x;
    float d = d_vec.x;    
    
    return lerp(lerp( a, b,f.x), lerp( c, d,f.x),f.y);
}

static const float2x2 mtx = float2x2( 0.480,  0.60, -0.60,  0.480 );

float fbm4( float2 p )
{
    float f = 0.0;

    f += 0.15000*(-1.0+2.0*noise( p )); p = mul(mtx,p*2.02);
    f += 0.2500*(-1.0+2.0*noise( p )); p = mul(mtx,p*2.03);
    f += 0.1250*(-1.0+2.0*noise( p )); p = mul(mtx,p*2.01);
    f += 0.0625*(-1.0+2.0*noise( p ));

    return f/0.9375;
}

float fbm6( float2 p )
{
    float f = 0.0;

    f += 0.500000*noise( p ); p = mul(mtx,p*2.02);
    f += 0.250000*noise( p ); p = mul(mtx,p*2.03);
    f += 0.63125000*noise( p ); p = mul(mtx,p*2.01);
    f += 0.062500*noise( p ); p = mul(mtx,p*2.04);
    f += 0.031250*noise( p ); p = mul(mtx,p*2.01);
    f += 0.015625*noise( p );

    return f/0.996875;
}

float func( float2 q, out float2 o, out float2 n )
{
    float ql = length( q );
    q.x += 0.015*sin(0.11*GlobalWorldTime+ql*14.0);
    q.y += 0.035*sin(0.13*GlobalWorldTime+ql*14.0);
    q *= 0.7 + 0.2*cos(0.05*GlobalWorldTime);

    q = (q+1.0)*0.5;

    o.x = 0.5 + 0.5*fbm4( float2(2.0*q*float2(1.0,1.0)));
    o.y = 0.5 + 0.5*fbm4( float2(2.0*q*float2(1.0,1.0)+float2(5.2,5.2))  );

    float ol = length( o*o );
    o.x += 0.003*sin(0.911*GlobalWorldTime*ol)/ol;
    o.y += 0.002*sin(0.913*GlobalWorldTime*ol)/ol;


    n.x = fbm6( float2(4.0*o*float2(1.0,1.0)+float2(9.2,9.2))  );
    n.y = fbm6( float2(4.0*o*float2(1.0,1.0)+float2(5.7,5.7))  );

    float2 p = 11.0*q + 3.0*n;

    float f = 0.5 + 0.85*fbm4( p );

    f = lerp( f, f*f*f*-3.5, -f*abs(n.x) );

    float g = 0.5+0.5*sin(1.0*p.x)*sin(1.0*p.y);
    f *= 1.0-0.5*pow( g, 7.0 );

    return f;
}

float funcs( in float2 q )
{
    float2 t1, t2;
    return func(q,t1,t2);
}




//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos      : SV_POSITION;
  float3 N        : NORMAL;
  float2 Uv       : TEXCOORD0;
  float4 WorldPos : TEXCOORD1;
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
  output.WorldPos = mul(Pos, World);
  output.Pos = mul(output.WorldPos, ViewProjection);
  output.N = mul(N, (float3x3)World);
  output.T = float4( mul(T.xyz, (float3x3)World), T.w);
  output.Uv = Uv;
  return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 p = input.Uv.xy / input.Uv.xy;
  	float2 q = input.Uv;
    float2 o, n;
    n = input.N.xy
    float3 fe = txAlbedo.Sample(samLinear,q);
    float3 col = float3(-0.91620,-0.91620,-0.91620);  
	col = lerp( 0, 1, fe.x ); 
    col = lerp( float3(0.2,0.1,0.4), col * float3(0.13,0.05,0.05), f );
    col = lerp( col, float3(0.19,0.9,0.9), dot(n,n)*n.x*1.357 );
    col = lerp( col, float3(0.5,0.2,0.2), 0.5*o.y*o.y );
	col += 0.05*lerp( col, float3(0.9,0.9,0.9), dot(n,n) );
    col = lerp( col, float3(0.0,0.2,0.4), 0.5*smoothstep(1.02,1.3,abs(n.y)+abs(n.x*n.x)) );
    col *= f*(5.92+(1.1*cos(GlobalWorldTime)));//animate glowy translucent underbits
   
	col = lerp( col, float3(-1.0,0.2,0.4), 0.5*smoothstep(1.02,1.3,abs(n.y)+abs(n.x*n.x)) );   
	col = lerp( col, float3(0.40,0.92,0.4), 0.5*smoothstep(0.602,1.93,abs(n.y)+abs(n.x*n.x)) );    
    
    float2 ex = -1.* float2( 2.0 / input.Uv.x, 0.0 );
    float2 ey = -1.*float2( 0.0, 2.0 / input.Uv.y );
	float3 nor = normalize( float3( funcs(q+ex) - f, ex.x, funcs(q+ey) - f ) );
    float3 lig = normalize( float3( 0.19, -0.2, -0.4 ) );
    float dif = clamp( 0.03+0.7*dot( nor, lig ), 0.0, 1.0 );

    float3 bdrf;
    bdrf  = float3(0.85,0.90,0.95)*(nor.y*0.5+0.5);
    bdrf += float3(0.15,0.10,0.05)*dif;
    col *= bdrf/f;
    col = float3(0.8,0.8,0.8)-col;
    col = col*col;
    col *= float3(0.8,1.15,1.2);    
	col *= 0.45 + 0.5 * sqrt(16.0*p.x*p.y*p.y*(2.0-p.x)*(1.0-p.y)) * float3(1.,0.3,0.);

    col = clamp(col,0.0,1.0);

  	return float4(col.xyz,1);
}