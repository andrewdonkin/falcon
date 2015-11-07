#include "shapes3.inc"

global_settings {
 //  ambient_light 0
  // ambient_light 0  radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

plane { y, 0
  pigment {
    checker color rgb <0,0,0>, colour rgb <1,1,1>
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
  cylinder {0, z, thick
  translate <0, 0, 1>}
  object{ Segment_of_Torus(1, thick, -45) rotate <0,90,0> translate <thick, 0, 1> rotate <0,0,90>}
  texture { pigment { color rgb<1,1,0> }}


}


#if (0)
light_source {
  <0,0,0>             // light's position (translated below)
  color rgb 1.0       // light's color
  area_light
  <0.5, 0, 0> <0, 0.5, 0>
  9, 9
  adaptive 1          // 0,1,2,3...
  //jitter              // adds random softening of light
  circular            // make the shape of the light circular
   orient              // orient light
    looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 diffuse 1 }}}
  translate <2, 1.5, -2>
}
#end

light_source { <2, 1.5, -2> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}


camera { location <1, 3, -3> look_at <0, 1, 0.5> }
