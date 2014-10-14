#default {
  texture {
    pigment { rgb 1 }
//    normal { bumps 1 }
    finish { ambient 0.1 }
  }
}

#include "hexdron.inc"

hexdron(0, 2*z, x+2*z, x,
        -y, z-y, 2*x+z-y, 2*x-y,
        0.2)


global_settings {
  // ambient_light 1
  ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
//  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

camera { location <3, 1, -5> look_at <0, 0, 0> }
