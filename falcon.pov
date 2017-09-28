#version 3.7;
#include "roughshapes.inc"
#include "hexdron.inc"
#include "falcon.inc"

global_settings {
   ambient_light 0
  ambient_light 0 radiosity {  } max_trace_level 200
  assumed_gamma 1
}

object { body }

light_source { <47, 9, -50> rgb 1 }
light_source { <-47, -9, -50> rgb 1 }
sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

camera { location <20, 15, -60> look_at <0, 0, 0> } // port ahead
//camera { location <0, 0, -50> look_at <0, 0, 0> } // mandibles
//camera { location <1, 0, -50> look_at <4.3, 0, -39> angle 20} // V12
//camera { location <40, 15, -10> look_at <0, 0, 0> } // ahead of port beam
//camera { location <50, -15, 10> look_at <0, 0, 0> } // aft of port beam, under
