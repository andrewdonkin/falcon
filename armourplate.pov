// Test bench for armourplate.inc

#version 3.7;
#include "armourplate.inc"

global_settings {
   // ambient_light 0
   // ambient_light 0  radiosity {  } max_trace_level 200
  assumed_gamma 1
}

plane { y, 0
  pigment {
    checker color rgb <0,0,0>, colour rgb <1,1,1>
  }
}

#if(0)
#local bigrad = 1.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.05;     // 1
#local tinyrad=thick/5;
#local rotwidth = 78;       // 1
#local rotdepth = 48;     // 1.3
union {
  sphere {0, bigrad}
  object { ArmourPlate1(bigrad, rotwidth, rotdepth, thick, tinyrad) }
  object { ArmourPlate1(bigrad, rotwidth, rotdepth, thick, tinyrad) rotate -rotdepth*x*1.1 }
  // a few stakes 
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, 0> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <       0, 0, -rotwidth> }
  #local Zclipper = vrotate(-x, -rotwidth * z);
  #local Xclipper = vrotate(-z, <rotdepth, 0, 0>);
  #local stake = vnormalize(vcross(Xclipper, Zclipper));
  cylinder { (bigrad - 1) * stake, (bigrad + 2*thick)*stake, 0.01 }
  //  translate -(bigrad - 1) * y
  rotate 90*y
  texture { pigment { color rgb<1,0.5,0> }}
}
camera { location <1, 3, -3> look_at <0, 1, 0.5> }
camera { location <2, 1, -4> look_at <0.4, 0.85, 0> angle 50 }
#end

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

#local bigrad = 1.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.15;     // 1
#local tinyrad=thick/20;
union {
  sphere {0, bigrad}
  object { BallArmour(bigrad, 15, 87, 35, thick, tinyrad) pigment { rgb<1,0,1>} }
  texture { pigment { color rgb<1,0.5,0> }}
  rotate 55*y
  translate bigrad*y
}

camera { location <0, bigrad*3, -bigrad*8> look_at <0.8, bigrad*1.3, 0> angle 26 }

light_source { <0.5, 1, -4> rgb 1
//  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}

