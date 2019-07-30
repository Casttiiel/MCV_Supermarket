//--------------------------------------------------------------------------------------
#include "common.fx"
#include "particles.inc"

#define NUM_PARTICLES_PER_THREAD_GROUP  16
#define OFFSET_NUM_PARTICLES_TO_DRAW    4

// --------------------------------------------------------------
struct TInstance {
  float3 pos;
  uint   unique_id;
  
  float3 dir;
  float  dummy1;

  float3 acc;
  float  dummy2;

  float3 prev_pos;
  float  dummy3;

  float4 color;

  float  scale;
  float  time_normalized;     // 0..1 When time reaches zero the particles dies.
  float  time_factor;         // real time adds to time_normalized by this factor
  float  dummy4;
};

struct TSystem {
  float  time_to_next_spawn;
  uint   num_particles_to_update;
  uint   next_unique_id;
  uint   dummy;
};

// ----------------------------------------------------------
// The spawn fn to customize
TInstance spawnParticle( uint unique_id ) {
  
  float2 rnd2 = hash2( unique_id ) * 2.0 - 1.0;
  float2 rnd3 = hash2( unique_id + 57 );
  float2 rnd4 = hash2( unique_id + 75 );
  float  duration = emitter_duration.x + ( emitter_duration.y - emitter_duration.x ) * rnd3.x;
  float  speed  = emitter_speed.x + ( emitter_speed.y - emitter_speed.x ) * rnd3.y;

  TInstance p;
  p.pos = emitter_center;
  p.prev_pos = p.pos;
  p.acc = float3(0,0,0);
  p.dir = emitter_dir;
  p.unique_id = unique_id;
  p.time_normalized = 0.0;
  p.time_factor = 1.0 / duration;
  p.scale = 1.0;
  p.color = float4(1,1,0,1);
  p.dummy1 = 0;
  p.dummy2 = 0;
  p.dummy3 = 0;
  p.dummy4 = 0;

  p.pos += float3( rnd2.x, rnd4.x, rnd2.y) * emitter_center_radius;
  p.dir += float3( rnd2.y, rnd4.y, rnd2.x) * emitter_dir_aperture;
  p.dir *= speed;

  return p;
}

// ----------------------------------------------------------
// The update fn to customize
void updateParticle( inout TInstance p ) {
  p.prev_pos = p.pos;
  p.color = sampleColor( p.time_normalized );
  p.dir += p.acc * GlobalDeltaTime;
  p.pos += p.dir * GlobalDeltaTime;
  /*if( p.pos.y < 0 ) {
    p.pos.y = -p.pos.y;
    p.dir.y = -p.dir.y;
  }*/
}

// --------------------------------------------------------------
// using a single thread to spawn new particles
[numthreads(1, 1, 1)]
void cs_particles_fire_spawn( 
  uint thread_id : SV_DispatchThreadID,
  RWStructuredBuffer<TInstance> instances : register(u0),
  RWStructuredBuffer<TSystem> system  : register(u1),
  RWByteAddressBuffer indirect_draw   : register(u2),
  RWByteAddressBuffer indirect_update : register(u3)
) {

  // We start from the num particles left in the prev frame
  uint   nparticles_active = indirect_draw.Load( OFFSET_NUM_PARTICLES_TO_DRAW );

  // Clear num instances for indirect draw call. At offset 4, set zero
  indirect_draw.Store( OFFSET_NUM_PARTICLES_TO_DRAW, 0 );

  // Get access to the max capacity of the buffer
  uint max_elements, bytes_per_instance;
  instances.GetDimensions( max_elements, bytes_per_instance );

  // Can we spawn particles?
  if( nparticles_active + emitter_num_particles_per_spawn < max_elements ) {

    system[0].time_to_next_spawn -= GlobalDeltaTime;
    if( system[0].time_to_next_spawn < 0) {
      system[0].time_to_next_spawn += emitter_time_between_spawns;

      // Spawn N
      uint unique_id = system[0].next_unique_id;
      for( uint i=0; i<emitter_num_particles_per_spawn; ++i ) {
        instances[nparticles_active] = spawnParticle(unique_id);
        ++nparticles_active;
        ++unique_id;
      }

      system[0].next_unique_id = unique_id;
    }
  }

  // Update DispatchIndirect 1st argument.
  uint nthread_groups = ( nparticles_active + NUM_PARTICLES_PER_THREAD_GROUP - 1 ) / NUM_PARTICLES_PER_THREAD_GROUP;
  indirect_update.Store(0, nthread_groups);
  indirect_update.Store(4, 1);
  indirect_update.Store(8, 1);
  system[0].num_particles_to_update = nparticles_active;
}

// --------------------------------------------------------------
[numthreads(NUM_PARTICLES_PER_THREAD_GROUP, 1, 1)]
void cs_particles_fire_update( 
  uint thread_id : SV_DispatchThreadID,
  StructuredBuffer<TInstance> instances : register(t0),
  StructuredBuffer<TSystem>   system    : register(t1),
  RWStructuredBuffer<TInstance> instances_active : register(u0),
  RWByteAddressBuffer indirect_draw : register(u2)
) {

  if( thread_id >= system[0].num_particles_to_update )
    return;

  TInstance p = instances[ thread_id ];

  // Has died?
  p.time_normalized += GlobalDeltaTime * p.time_factor;
  if( p.time_normalized >= 1 ) 
    return;

  // Call the specific method to update each particle
  updateParticle( p );

  // Update indirect draw call args
  uint index;
  indirect_draw.InterlockedAdd( OFFSET_NUM_PARTICLES_TO_DRAW, 1, index );

  // Save in the nexts buffer
  instances_active[index] = p;
}

//-------------------------`-------------------------------------------------------------
struct v2p {   // Vertex to pixel
  float4 Pos   : SV_POSITION;
  float2 Uv    : TEXCOORD0;
  float4 Color : COLOR;
};

struct VS_INPUT {   // Input from billboard mesh
  float4 Pos   : POSITION;
  float2 Uv    : TEXCOORD0;
  float4 Color : COLOR;         // Not used anymore
};

//--------------------------------------------------------------------------------------
v2p VS(
  in VS_INPUT input
, in uint InstanceID : SV_InstanceID
, StructuredBuffer<TInstance> instances_active : register(t0)
)
{
  TInstance instance = instances_active[ InstanceID ];

  // orient billboard to camera
  float3 localPos = input.Pos.x * CameraLeft
                  + input.Pos.y * CameraUp;
  float3 p = instance.pos + localPos * instance.scale;
  /* 

  // Strech based on direction
  float3 world_dir = instance.pos - instance.prev_pos;
  float3 center = ( instance.pos + instance.prev_pos ) * 0.5f;

  // orient billboard to camera
  float3 localPos = input.Pos.x * CameraLeft * 0.1;
  float3 p = center + world_dir * 2 * ( input.Pos.y * 2 - 1.0 ) + localPos;
  */

  // 
  v2p output;
  output.Pos = mul( float4(p,1.0), ViewProjection );
  output.Uv = input.Uv;
  output.Color = instance.color;
  return output;
}

//--------------------------------------------------------------------------------------
float4 PS2(v2p input) : SV_Target {
  float4 red = float4(1,0,0,1);
  float4 yellow = float4(1,1,0,1);
  float4 orange = float4(1,0.2f,0,1);
  float4 blue = float4(0,0,1,1);
  float4 green = float4(0,1,0,1);

  //for color
  const float _offset = 0.0f;
  //for rim
  const float _edge = 0.7f;
  const float _hard = 5.0f;
  //for shape
  const float _height = 0.70f;
  //for distortion
  const float _scrollX = 0.0f;
  const float _scrollY = 0.2f;
  const float _distort = 0.3f;

  float4 gradientMain = lerp(red, yellow, input.Uv.y + _offset);
  float4 gradientTint = lerp(blue, green, input.Uv.y + _offset);
  float4 gradientBlend = lerp(float4(2,2,2,2), float4(0,0,0,0), (input.Uv.y + _height));

  float4 distortion = txRoughness.Sample(samLinear, input.Uv) * _distort;
  float4 voronoi_noise = txNormal.Sample(samLinear, float2((input.Uv.x - GlobalWorldTime * _scrollX) + distortion.g  ,(input.Uv.y - GlobalWorldTime * _scrollY) + distortion.r));
  voronoi_noise.a = voronoi_noise.z;
  float shapetex = txAlbedo.Sample(samLinear, input.Uv).x;
  
  voronoi_noise += gradientBlend;
  //voronoi_noise = 1 - (voronoi_noise * _height + (1 - (shapetex * _hard)));

  float4 flame = saturate(voronoi_noise.a * _hard);
  float4 flamecolored = flame * yellow;
  float4 flamerim = saturate((voronoi_noise.a + _edge) * _hard) - flame;

  float4 flamecolored2 = flamerim * red;
  float4 finalcolor = flamecolored + flamecolored2;

  //return input.Color * 4;
  return finalcolor; // + float4( 1,1,1,0);
}



float4 PS(v2p input) : SV_Target {
  float4 red = float4(1,0,0,1);
  float4 yellow = float4(1,1,0,1);
  float4 orange = float4(1,0.2f,0,1);
  float4 blue = float4(0,0,1,1);
  float4 green = float4(0,1,0,1);

  //for color
  const float _offset = 0.0f;
  //for rim
  const float _edge = 0.7f;
  const float _hard = 5.0f;
  //for shape
  const float _height = 0.70f;
  //for distortion
  const float _scrollX = 0.0f;
  const float _scrollY = 0.4f;
  const float _distort = 0.15f;
  //for visuals
  const float _threshold = 0.9f;
  const float _rim = 0.1f;
  const float _scale = 0.4f;

  float4 distortion = txRoughness.Sample(samLinear, input.Uv) * _distort;
  float4 voronoi_noise = txNormal.Sample(samLinear, float2((input.Uv.x - GlobalWorldTime * _scrollX) + distortion.g  ,(input.Uv.y - GlobalWorldTime * _scrollY) + distortion.r) * float2(_scale,_scale));
  voronoi_noise.a = voronoi_noise.z;
  float shapetex = txAlbedo.Sample(samLinear, input.Uv).a;
  
  voronoi_noise += shapetex * _threshold * 0.6f;
  float4 final_voronoi = voronoi_noise;

  float flame = final_voronoi.x > _threshold;
  float4 flamecolored = flame * yellow;

  float4 flamerim = (final_voronoi.x > _threshold - _rim) - flame;
  float4 flamecolored2 = flamerim * orange;
  float4 finalcolor = flamecolored + flamecolored2;
  finalcolor *= length(input.Uv - float2(0.5f,0.5f)) < 0.4f;

  //return input.Color * 4;
  return finalcolor; // + float4( 1,1,1,0);
}
