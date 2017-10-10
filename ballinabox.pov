#version 3.7;
#include "stdafx.inc"
#default {
  texture {
    pigment { rgb <1,0,0> }
    // normal { bumps 1/20 scale 1/100 }
    finish { ambient 0.1 reflection 0 phong 1 }
  }
}

global_settings {
   ambient_light 1
  // ambient_light 0 radiosity {  }
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
    pigment{ checker color rgb <1,1,1>, color rgb <0.3,0.3,0.3> }
    //pigment{ color rgb <1,1,1> }
    finish { phong 0 reflection 0 } 
  }
}  
plane {-z, -20
  texture {
    pigment{ checker color rgb <1,1,1>, color rgb <0.3,0.3,0.3> }
    //pigment{ color rgb <1,1,1> }
    finish { phong 0 reflection 0 } 
  }
}  

// #declare CHEAP=1;
#declare DEBUG=1;
#include "hexdron.inc"

#local TLF=<1.5, 2, 1>;
#local TLR=<1, 2, 2>;
#local TRR=<3, 2, 2>;
#local TRF=<2.4, 2, 1>;
#local BLF=<0.5, 1, 1>;
#local BLR=<0, 1, 2>;
#local BRR=<2.5, 1, 2>;
#local BRF=<1.9, 1, 1>;

union {

  #local R=1/200;
  //cylinder{BLF, TLF, R} 
  cylinder{BLR, TLR, R}
  cylinder{BLF, BLR, R} cylinder{BLR, BRR, R}
  cylinder{BLF, BRF, R} cylinder{BRR, BRF, R}
  cylinder{TRR, BRR, R} cylinder{BRF, TRF, R}
  cylinder{TRR, TLR, R} //cylinder{TLF, TLR, R}
  cylinder{TRR, TRF, R} cylinder{TRF, TLF, R}

  cylinder{TLR, TRF, R} cylinder{TRR, TLF, R}
  cylinder{BLR, TLF, R} cylinder{TLR, BLF, R}
  
  cylinder{BLF, TRF, R}

  difference {
    _hexdron2(TLF, TLR, TRR, TRF,  BLF, BLR, BRR, BRF, 0.05,)
    // Take a bite out of it just to show that it is CSG-able
    #declare fn_Pigm=function {
      pigment {
        agate
        color_map {
          [0 color rgb 0]
          [1 color rgb 1]
        }
      }
    }
    isosurface {
      // function { f_sphere(x, y, z, 1) - f_noise3d(x * 10, y * 2, z * 2) * 0.5 }
      function { sqrt(x*x+y*y+z*z) -1 - fn_Pigm(x * 2, y /5, z /5).grey * 0.1 }
      max_gradient 4
      all_intersections
      contained_by { box { -2, 2 } }
      scale <0.5, 0.3, 0.3>
      translate <1.2, 1.1, 0.8>
    }
    texture {
      pigment {rgbf <0.6, 0, 0, 0>}
      finish { ambient 0.1 reflection 0.0 specular 0.1 roughness 0.0001}
    }
  }

  translate <-1, -1, -1>
  //rotate <50, -30, 0>
  rotate <0, 0, 0>
  rotate <-20, 0, 0>
  texture {
    pigment {rgbf <1, 1, 1, 0.8>}
    finish { ambient 0.1 reflection 0.3 specular 0.1 roughness 0.0001}
  }
  interior {ior 1.3}
  translate y
}

camera { location <0, 2, -8> look_at <0.5, 1.3, 0> angle 21}
