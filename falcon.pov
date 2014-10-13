#include "falcon.inc"

global_settings {
  // ambient_light 1
  ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

object { body }

sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

camera { location <4, 4, -50> look_at <0, 0, 0> }
