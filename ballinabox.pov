#version 3.7;
#include "math.inc"
#include "stdafx.inc"
#default {
  texture {
    pigment { rgb <1,0.5,0> }
    //normal { bumps 1/20 scale 1/100 }
    finish { ambient 0.1 reflection 0.1 phong 1 }
  }
}

global_settings {
   ambient_light 1
  //ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

light_source { <8, 8, -40> rgb 1 }

//sky_sphere {
  // pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  //pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  //pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
//}

plane {y, 0
  texture {
    //pigment{ checker color rgb <1,1,1>, color rgb <0.3,0.3,0.3> }
    pigment{ color rgb <1,1,1> }
    finish { phong 1 reflection 0.4 } 
  }
}  

// #declare CHEAP=1;
#declare DEBUG=1;
#include "hexdron.inc"

#local TLF=<1.5, 2, 1>;
#local TLR=<1, 2, 2>;
#local TRR=<3, 2, 2>;
#local TRF=<2.5, 2, 1>;
#local BLF=<0.5, 1, 1>;
#local BLR=<0, 1, 2>;
#local BRR=<2.5, 1, 2>;
#local BRF=<2, 1, 1>;

union {

//  hexdron(TLF, TLR, <1, 1, 1>, TRF,
//          BLF, BLR, <1, 0, 1>, BRF,
//           0  // edge radius
//           , 0 // use saddle?
//  )
  cylinder{BLF, TLF, 1/100} cylinder{BLR, TLR, 1/100}
  cylinder{BLF, BLR, 1/100} cylinder{BLR, BRR, 1/100}
  cylinder{BLF, BRF, 1/100} cylinder{BRR, BRF, 1/100}
  cylinder{TRR, BRR, 1/100} cylinder{BRF, TRF, 1/100}
  cylinder{TRR, TLR, 1/100} cylinder{TLF, TLR, 1/100}
  cylinder{TRR, TRF, 1/100} cylinder{TRF, TLF, 1/100}

  #local R=0.05;
  
  //cornerball(BLF, BLR, TLF, BRF, R)
  //cornerball(TLF, TLR, TRF, BLF, R)
  //cornerball(TLF, TRF, BLF, TLR, R)

    difference {
    _hexdron2(TLF, TLR, TRR, TRF,  BLF, BLR, BRR, BRF, R,)
    cylinder {<1.5,1.5,-10>, <1.5,1.5,10>, 0.1}
    sphere {1, 0.5}
    }

  translate <-1, -1, -1>
  //rotate <50, -30, 0>
  rotate <0, -0, 0>
  rotate <-20, 0, 0>
  texture {
    pigment {rgbf <1, 0.5, 0, 0>}
    finish { ambient 0.1 reflection 0.0 specular 0.1 roughness 0.0001}
  }
  translate y
}

camera { location <0, 2, -8> look_at y angle 27}

