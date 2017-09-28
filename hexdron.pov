#version 3.7;
#default {
  texture {
    pigment { rgb 1 }
    normal { bumps 1/20 scale 1/100 }
    finish { ambient 0.1 phong 1 }
  }
}

#include "hexdron.inc"

#for (I, 0, 1)
  object {
    hexdron(0, <0,0.3,1>, <0.7, -0.1, 1>, <1,-0.1,-0.2>,
            <0,-1,0>, <0.1,-1,2>, <1.1, -0.9, 1> <1,-1,0>  // commas optional I guess
             0.05  // edge radius
             , I // use saddle?
    ) translate <-1 + I*2, 1,0,>
  }
#end

//object {  pollywantahedron(1.5, 1, 1, 30, 10, 5, 15, 5, 0.1)  translate -y/2 }
//camera { location <1.1, 0.5, 0.7> look_at <0.5, 0.3, 0.8> }

//object {  pollywantahedron(1.5, 1, 1, /* toein */ 4, /* camber */ 6, 20, 8, 10, 0.1)  translate -y/2 }
//camera { location <1.6, 0.5, 0.6> look_at <1.30, 0.44, 0.88> angle 30}

#if(1)
object {  pollywantahedron(2, 1, 1, /* toein */ 4, /* camber */ 22, 20, 12, 4, 0.01)
    translate <-1, 0, 2>
}
#end

//object { saddleup (<0,0,0.7> <0,-0.7,0> <0.7,0,0> <0,0.7,0>) }

// end of example 


camera { location <5, 3, -8> look_at <0.5, 0.5, 0.5> angle 20}


global_settings {
   ambient_light 1
  //ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

light_source { <100, 50, -100> rgb 1 }

sky_sphere {
  // pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  // pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

