#version 3.7;
#include "math.inc"
#include "stdafx.inc"
#default {
  texture {
    pigment { rgb <0,0,1> }
    // normal { bumps 1/20 scale 1/100 }
    finish { ambient 0.0 reflection 0 phong 0 }
  }
}

global_settings {
   ambient_light 1
  // ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

light_source { <8, 5, -8> rgb 1 }

plane {y, -1
  texture {
    pigment{ checker color rgb <1,1,1>, color rgb <0.3,0.3,0.3> }
    //pigment{ color rgb <1,1,1> }
    finish { reflection 0 } 
  }
}  
plane {-z, -5
  texture {
    pigment{ checker color rgb <1,1,1>, color rgb <0.3,0.3,0.3> }
    //pigment{ color rgb <1,1,1> }
    finish { reflection 0 } 
  }
}  

// #declare CHEAP=1;
#declare DEBUG=1;

union {

  #declare fn_Pigm=function {
    pigment {
      agate
      color_map {
        [0 color rgb 0]
        [1 color rgb 1]
      }
    }
  }

  union {
    cylinder {0, <0, 0.1, 0>, 1}
    isosurface {
      // function { f_sphere(x, y, z, 1) - f_noise3d(x * 10, y * 2, z * 2) * 0.5 }
      function {f_torus(x, y, z, 1, 0.1)
        + f_noise3d(x*50, y*10, z*50) / 20 *
            //OneAtZero(y/0.10, 1)
            OneAtZero(sqrt(x*x+z*z)-1.1, 0.09)
      }
      max_gradient 4
      all_intersections
      contained_by { box { <-1.2, -0.2, -1.5>, <1.2, 0.2, 1.5> } }
      //    scale <0.5, 0.3, 0.3>
    }
    rotate -20*x
    translate <0, 1, 0>
    texture {
      pigment {rgbf <0.3, 0.3, 1, 0>}
      finish { phong 1 phong_size 60 reflection 0.2}
    }
  }

  union {
    disc {<0, 0.1, 0>, y, 2, 1}
    isosurface {
      // function { f_sphere(x, y, z, 1) - f_noise3d(x * 10, y * 2, z * 2) * 0.5 }
      function {f_torus(x, y, z, 1, 0.1)
        //      - f_noise3d(x*20, y*2, z*20) / 10
        + fn_Pigm(x*2, y/2, z*2).grey / 30 *
        // OneAtZero(y, 0.1)
        OneAtZero(sqrt(x*x+z*z)-0.9, 0.1)
      }
      max_gradient 4
      all_intersections
      contained_by { box { <-1.2, -0.2, -1.5>, <1.2, 0.2, 1.5> } }
      //    scale <0.5, 0.3, 0.3>
      //    translate <1.2, 1.1, 0.8>
    }
    texture {
      pigment {rgbf <0.3, 1, 0.3, 0>}
      finish { specular 1 roughness 0.01 reflection 0.2 }
    }
  }

//  translate <-1, -1, -1>
  //rotate <50, -30, 0>
  rotate <0, 0, 0>
//  rotate <-20, 0, 0>
  texture {
    pigment {rgbf <1, 1, 1, 0.8>}
    finish { ambient 0.1 reflection 0.3 specular 0.1 roughness 0.0001}
  }
}

camera { location <0, 2, -8> look_at <0, 0.2, 0> angle 21}
