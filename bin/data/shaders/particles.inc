float4 sampleColor( float t ) {

  // Convert range 0..1 to 0..7
  float time_in_color_table = t * ( 8 - 1 );

  // if time_in_color_table = 1.2.      color_entry = 1., color_amount = 0.2
  float color_entry;
  float color_amount = modf( time_in_color_table, color_entry);
  float4 color_over_time = psystem_colors_over_time[ color_entry ] * ( 1 - color_amount )
                         + psystem_colors_over_time[ color_entry + 1 ] * ( color_amount );

  return color_over_time;
}

