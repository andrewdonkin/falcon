#default {
  texture {
    pigment { rgb 1 }
//    normal { bumps 1 }
    finish { ambient 0.1 }
  }
}

#include "hexdron.inc"

//hexdron(0, 2*z, x+2*z, x,
//        -y, z-y, 2*x+z-y, 2*x-y,        0.2)


//object {  pollywantahedron(1.5, 1, 1, 30, 10, 5, 15, 5, 0.1)  translate -y/2 }
//camera { location <1.1, 0.5, 0.7> look_at <0.5, 0.3, 0.8> }

//object {  pollywantahedron(1.5, 1, 1, /* toein */ 4, /* camber */ 6, 20, 8, 10, 0.1)  translate -y/2 }
//camera { location <1.6, 0.5, 0.6> look_at <1.30, 0.44, 0.88> angle 30}


object {  pollywantahedron(1.5, 1, 1, /* toein */ 4, /* camber */ 22, 20, 12, 4, 0.1)  translate -y/2 }
camera { location <1.3, 0.6, 0.4> look_at <1.05, 0.48, 0.85> angle 30}
//camera { location <1.5, 0.6, -0.6> look_at <1.3, 0.5, 1> angle 30}


global_settings {
   ambient_light 1
  //ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

light_source { <10, 50, -100> rgb 1 }

sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
//  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
   pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

