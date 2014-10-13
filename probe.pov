#include "colors.inc"

global_settings {
  // ambient_light 1
     ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
  }

sphere {0, 1 texture { 
 pigment {color White}
 finish { reflection 0.3}
 }
 
 }


camera { location <0, 3, -15> look_at <0, 0, 0> angle 150 }

sky_sphere {
  pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}
