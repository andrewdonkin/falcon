// Macro for generating a four-sided tile that would fit on a sphere.
// The four upper edges and four edges along the sphere's radius are rounded.

#version 3.7;
#include "math.inc"
#include "transforms.inc"

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

#local bigrad = 1.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.1;     // 1
#local tinyrad=thick/4;

#local rotwidth = 58;       // 1
#local rotdepth = 38;     // 1.3

#local Frontplane = plane {  z 0 }
#local Leftplane =  plane {  x 0 }
#local Rearplane =  plane { -z 0 rotate  rotdepth * x }
#local Rightplane = plane { -x 0 rotate -rotwidth * z }

union {
  difference { // quadrilateral cut out of a ball, sharp edges
    sphere { 0, bigrad + thick }
    sphere { 0, bigrad }
    Frontplane Rearplane Leftplane Rightplane
  }

  // The far corner is always the tricky one
  #local Rightclippernorm = vrotate(-x, -rotwidth * z);  // surface normal of right clipper
  #local Rearclippernorm = vrotate(-z, <rotdepth, 0, 0>);  // surf normal of back clipper
  #local Corner4Vec = vnormalize(vcross(Rearclippernorm, Rightclippernorm));  // Corner4 runs up their intersection
  // Corner4Vec is now a vector indicating the corner at the intersection of the two clipping planes
  #local Corner4IntAngle = VAngleD(Rightclippernorm, Rearclippernorm);
  #warning concat("Corner4IntAngle=", str(VAngleD(Rightclippernorm, Rearclippernorm), 0, 0))

  // four balls to join the rolled-off edges
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y rotate -rotwidth * z}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y rotate  rotdepth * x}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * Corner4Vec }

  #local Rearplaneadjusted = plane { -z 0 rotate  rotdepth * x 
    Axis_Rotate_Trans(Corner4Vec, -(90-Corner4IntAngle))  // trust me, is genius
    texture { pigment { color rgb<1,0.5,0> } }
  };

  #local Rightplaneadjusted = plane { -x 0 rotate -rotwidth * z
    Axis_Rotate_Trans(Corner4Vec, 90-Corner4IntAngle)  // even more genius without the negation
    texture { pigment { color rgb<1,0.5,0> } }
  };
 
  // slightly larger quadrilateral cut out of a ball with a slightly smaller radius

  // front
  difference {
    union {
      sphere { 0, bigrad + thick - tinyrad }
      torus {bigrad+thick-tinyrad, tinyrad rotate 90*x}
    }
    sphere { 0, bigrad }
    plane {  z, -tinyrad }
    plane { -z, -tinyrad rotate rotdepth * x}
    Leftplane
    Rightplane
  }
  // left
  difference {
    union {
      sphere { 0, bigrad + thick - tinyrad }
      torus {bigrad+thick-tinyrad, tinyrad rotate 90*z}
    }
    sphere { 0, bigrad }
    plane {  x, -tinyrad }
    plane { -x, -tinyrad rotate -rotwidth * z }
    Rearplane
    Frontplane
  }

  // right
  difference {
    union {
      sphere { 0, bigrad + thick - tinyrad }
      torus {bigrad+thick-tinyrad, tinyrad rotate (90-rotwidth)*z}
    }      
    sphere { 0, bigrad }
    plane {  x, -tinyrad }
    plane { -x, -tinyrad rotate -rotwidth * z }
    Rearplaneadjusted
    Frontplane
    //xclippers
  }

  // rear
  difference {
    union {
      sphere { 0, bigrad + thick - tinyrad }
      torus {bigrad+thick-tinyrad, tinyrad rotate (90+rotdepth)*x}
    }
    sphere { 0, bigrad }
    plane {  z, -tinyrad }
    plane { -z, -tinyrad rotate rotdepth * x}
    Leftplane
    Rightplaneadjusted
  }

  // four little posts, along big radii, that form the rounded-off "vertical" corners of our tile
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad }
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate -rotwidth * z}
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate  rotdepth * x}
  cylinder { (bigrad) * Corner4Vec, (bigrad + thick - tinyrad) * Corner4Vec, tinyrad }

  texture { pigment { color rgb<1,1,1> }}  
//  translate -(bigrad - 1 ) * y
  
  rotate 90*y
}


union {// a few stakes 
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, 0> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <       0, 0, -rotwidth> }
  
  #local Zclipper = vrotate(-x, -rotwidth * z);
  #local Xclipper = vrotate(-z, <rotdepth, 0, 0>);
  #local stake = vnormalize(vcross(Xclipper, Zclipper));
  
  cylinder { (bigrad - 1) * stake, (bigrad + 2*thick)*stake, 0.01 }


//  translate -(bigrad - 1) * y

  rotate 90*y


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
camera { location <2, 1, -4> look_at <0.4, 0.85, 0> angle 50 }
