#include "hexdron.inc"
#include "falcon.inc"

global_settings {
   ambient_light 0
//  ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

object { body }

light_source { <47, 9, -40> rgb 1 }
sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

camera { location <44, 4, -5> look_at <0, 0, 0> }
