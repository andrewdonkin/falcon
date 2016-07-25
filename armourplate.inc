#include "math.inc"
#include "transforms.inc"

// Macro for generating a four-sided tile that would fit on a sphere
//  bounded by lines of longitude and latitude.
// The four upper edges described by moving the radius along the lines
//  of lat and long, and the four edges along the sphere's radius itself
//  are rounded
//  with any luck.
// Dimensions are specified in degrees: two latitudes and a longitudinal width.
//  Latitudes start at zero at the north pole and increase as they head south.
//  One edge of plate will be at +x, other will be at the specified width
//   clockwise about y for positive values (POV's natural rotation).

#macro BallArmour(Bigrad,      // radius of the sphere this will sit on
                  Lattop,      // latitude of plate's top edge
                  Latbot,      // latitude of plate's lower edge, > Lattop
                  Width,       // of plate in degrees
                  Thick,       // ness of the plate
                  Tinyrad      // radius of smoothing
                 )

// Ball armour.  Heh.  Heh.

#if (Lattop >= Latbot)
# error "nuh"
#end

#local _Width = abs(Width);

#local Rightclipper = plane { -z, 0}
#local Leftclipper  = plane { z, 0 rotate _Width * y}
#local Topclipper = plane {-y, -Bigrad * sin(radians(90-Lattop))}
#local Botclipper = plane { y, Bigrad * sin(radians(90-Latbot))}

union
{
  difference
  {
    sphere {0, Bigrad + Thick}
    sphere {0, Bigrad}
    Leftclipper
    Rightclipper
    Topclipper
    Botclipper
  }

  //difference {
    torus {sin(radians(Lattop)) * (Bigrad + Thick - Tinyrad), Tinyrad
           translate sin(radians(90-Lattop)) * (Bigrad + Thick - Tinyrad) * y
           }
    torus {sin(radians(Latbot)) * (Bigrad + Thick - Tinyrad), Tinyrad
           translate sin(radians(90-Latbot)) * (Bigrad + Thick - Tinyrad) * y
           }
    torus {Bigrad + Thick - Tinyrad, Tinyrad
           rotate 90*x
           }
    torus {Bigrad + Thick - Tinyrad, Tinyrad
           rotate 90*x + _Width*y
           }

  #if (Width < 0)
    scale <1, 1, -1>
  #end

}

#end


// Macro for generating a four-sided tile that would fit on a sphere.
// The four upper edges and four edges along the sphere's radius are rounded.
// Dimensions are specified in degrees.
// One corner (front left) is at x=0 y=radius z=0.
// Rear left corner is at x=0 y=cosine(depth) z=sine(depth), times radius
// Front right corner is at x=sine(width) y=cosine(width) z=0
// Those three are all 90-degree corners.
// Rear right corner is a total bastard.  As the dimensions increase
//  toward 90 degrees the angle widens, and the corner eventually vanishes
//  to form a spinnaker.

#macro ArmourPlate1(bigrad,      // radius of sphere this will sit on
                    rotwidth,    // how far down to the right tile goes, in degrees
                    rotdepth,    // how far down to the rear tile goes, degrees
                    thick,       // thickness of tile
                    tinyrad      // radius of smoothed edges (increases size of tile)
                   )

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
  //#warning concat("Corner4IntAngle=", str(VAngleD(Rightclippernorm, Rearclippernorm), 0, 0))

  // four little posts, along big radii, that form the rounded-off "vertical" corners of our tile
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad }
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate -rotwidth * z}
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate  rotdepth * x}
  cylinder { (bigrad) * Corner4Vec, (bigrad + thick - tinyrad) * Corner4Vec, tinyrad }

  // four balls to cap those posts and join the rolled-off edges
  sphere { (bigrad + thick - tinyrad) * y, tinyrad}
  sphere { (bigrad + thick - tinyrad) * y, tinyrad rotate -rotwidth * z}
  sphere { (bigrad + thick - tinyrad) * y, tinyrad rotate  rotdepth * x}
  sphere { (bigrad + thick - tinyrad) * Corner4Vec, tinyrad}

  #local Rearplaneadjusted = plane { -z 0 rotate  rotdepth * x 
    Axis_Rotate_Trans(Corner4Vec, -(90-Corner4IntAngle))  // trust me, is genius
    texture { pigment { color rgb<1,0.5,0> } }
  };

  #local Rightplaneadjusted = plane { -x 0 rotate -rotwidth * z
    Axis_Rotate_Trans(Corner4Vec, 90-Corner4IntAngle)  // even more genius without the negation
    texture { pigment { color rgb<1,0.5,0> } }
  };
 
  // Slightly larger quadrilateral cut out of a ball with a slightly smaller radius
  // Fills in the tile below the torii which make the smoothed top edge

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

}
#end

