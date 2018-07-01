#version 3.8;
#include "skirtbox.inc"
#include "stdafx.inc"
#include "shapes3.inc" // segment_of_torus

global_settings {
  ambient_light 0 //radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

#local floor_tex = texture {
    pigment {checker color rgb <0.3, 0.3, 1>, colour rgb <0.8,0.8,1>}
    finish { phong 1 reflection 0.1 }

  };

plane { y, 0
  texture {
    floor_tex
  }
}

#local bigrad = 77.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.1;
#local rotwidth = 1;
#local rotdepth = 1.3;

union {// a few stakes 
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, 0> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <       0, 0, -rotwidth> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, -rotwidth> }
  translate -(bigrad - 1) * y
  texture { pigment { color rgb<1,1,0> }}
}
union {
  sphere {0, thick translate <0, thick, 0>}
  cylinder {0, z, thick translate <0, 0, 1>}
  object{ Segment_of_Torus(1, thick, -45) rotate <0,90,0> translate <thick, 0, 1> rotate <0,0,90>}
  texture { pigment { color rgb<1,1,0> }}
}

#if (1)
union {
  object {Skirted_box2(<1, 1, 1>, 0.03, 1)} // origin to <1,1,1>
  object {Skirted_box_y(<-0.4, 0.5, -0.4>, <0.4, 0.55, 0.4>,
    0.1, 0.02, 0.01, 0)} // big rad, top rad, skirt rad, filled
  object {Skirted_box_y(<-0.2, 0.5, -0.2>, <0.2, 0.6, 0.2>,
    0.15, 0.01, 0.01, 1) rotate 20*y} // big rad, top rad, skirt rad, filled
  texture { floor_tex}

  translate <0.75, 0, 0.5>
  texture { floor_tex}
//  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
}
#end

// These two should be identical
object {
  Skirted_box(<0.7, 0.5, 1>, 0.03, 0.1, 0*y)
  translate <1.5, 0, 0>
  texture { floor_tex}
//  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
  }
object {
  Skirted_box_y(o, <0.5, 0.5, 0.8>, 0.03, 0.03, 0.1, 1)
  translate <2.5, 0, 0.1>
  texture { floor_tex}
//  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
  }


#if (1)
// hollow, to form a wall around another
object {
  Skirted_box_y(<-0.1, 0, -1.1>, <0.9, 0.3, -0.6>,
    0.2, 0.03, 0.02, 0) // big rad, top rad, skirt rad, filled
  texture { floor_tex}
}
object { // solid, inside the wall
  Skirted_box_y(<0.1, 0, -1>, <0.7, 0.4, -0.7>,
    0.1, 0.03, 0.02, 1) // big rad, top rad, skirt rad, filled
  texture { floor_tex}
}
object { // half a sausage sitting on top
  Skirted_blister(0.3, // Z length
  0.05, 0.01 // large and skirting radii
   )
  rotate 100*y
  translate <0.25, 0.4, -0.85>
  texture { floor_tex }
}



object {
  Skirted_lozenge_y(1, 0.25,  // z, y
    0.2, // end rad
    0.03, // top rad
    0.02, // skirt
    0) // filled
  rotate 90*y
  translate <1.5, 0, -0.8000001>
  texture { floor_tex }
}
object {
  Skirted_lozenge_y(0.7, 0.25, 0.1, // end rad
    0.03, // top rad
    0.02, // skirt
    1) // filled
  rotate 90*y
  translate <1.5, 0, -0.8>
  texture { floor_tex }
}


object {
  Skirted_blister(1, // Z length
  0.4, 0.2 // large and skirting radii
   )
  rotate 90*y
  translate <1, 0, -1.5>
  texture { floor_tex }
}
#end

#if (1)
light_source {
  <0,0,0>             // light's position (translated below)
  color rgb 1.0       // light's color
//  area_light
//  <0.4, 0, 0> <0, 0.4, 0>
//  18, 18
//  adaptive 1          // 0,1,2,3...
//  jitter              // adds random softening of light
//  circular            // make the shape of the light circular
//   orient              // orient light
  //  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 diffuse 1 }}}
  translate <2, 1.5, -2>
}
#else

light_source { <2, 1.5, -2> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}
#end

camera { location <-0.1, 3, -4> look_at <1.2, 0, 0> angle 40}
//camera { location <0.1, 3, -4> look_at <2.5, 0, 0.5> angle 40}
//camera { location <-0.1, 3, -4> look_at <2, 0, -0.9> angle 5}
