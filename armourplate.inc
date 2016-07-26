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
// What angle do we need to increase the height by to include the slight
// swelling caused by Tinyrad?  See calculation for "littleangle" on paper.
#local Latdelta = degrees(asin(Tinyrad / (Bigrad + Thick - Tinyrad)));
#local Latdelta_r = asin(Tinyrad / (Bigrad + Thick - Tinyrad));

#if (Latbot + Latdelta >= 88)
# error concat ("nuh ", str(Latbot + Latdelta, 0, 2));
#end

#local Rightclipper = plane { -z, 0}
#local Leftclipper  = plane { z, 0 rotate _Width * y}

// Righer, Lefter clippers clip the sphere wider to accommodate round-off
#local Righterclipper = plane { -z, Tinyrad}
#local Lefterclipper  = plane { z, -Tinyrad rotate _Width * y}

#local Topclipper = union {
    plane {-y,  cos(radians(Lattop)) * -(Bigrad + Thick) }
    cone {0, 0, cos(radians(Lattop)) *  (Bigrad + Thick) * 1.001 * y,
                sin(radians(Lattop)) *  (Bigrad + Thick) }
}
#local Botclipper = intersection {
    plane { y,  cos(radians(Latbot)) * (Bigrad + Thick)}
    cone {0, 0, cos(radians(Latbot)) * (Bigrad + Thick) * 1.001 * y,
                sin(radians(Latbot)) * (Bigrad + Thick) inverse}
}
// Topper, Botter clippers clip the sphere taller to accommodate round-off.
// Trouble is they fit perfectly with conical posts.  The left and right
// clippers do not.
#local Topperclipper = union {
  plane {-y,  cos(radians(Lattop)-Latdelta_r) * -(Bigrad + Thick)}
  cone {0, 0, cos(radians(Lattop)-Latdelta_r) *  (Bigrad + Thick) * 1.001 * y,
              sin(radians(Lattop)-Latdelta_r) *  (Bigrad + Thick) }
}
#local Botterclipper = intersection {
  plane { y,  cos(radians(Latbot)+Latdelta_r) * (Bigrad + Thick)}
  cone {0,
           // hard to explain why I have to set a radius on the bottom of the cone here.  See paper.
           // It means that as Latbot+Latdelta approaches 90, rounding errors appear as artifacts
           Tinyrad/cos(radians(Latbot)+Latdelta_r),
           cos(radians(Latbot)+Latdelta_r) * (Bigrad + Thick) * y,
           sin(radians(Latbot)+Latdelta_r) * (Bigrad + Thick) inverse}
}

union
{
  // The armour plate, full thickness, sharp edges
  difference
  {
    sphere {0, Bigrad + Thick}
    sphere {0, Bigrad}
    Leftclipper
    Rightclipper
    Topclipper
    Botclipper
  }

  // Four rolled-off edges:
  // Top
  difference {
    torus {sin(radians(Lattop)) * (Bigrad + Thick - Tinyrad), Tinyrad
           translate cos(radians(Lattop)) * (Bigrad + Thick - Tinyrad) * y }
    Leftclipper
    Rightclipper
  }
  // Bottom
  difference {
    torus {sin(radians(Latbot)) * (Bigrad + Thick - Tinyrad), Tinyrad
           translate cos(radians(Latbot)) * (Bigrad + Thick - Tinyrad) * y}
    Leftclipper
    Rightclipper
  }
  // Through X axis, which is the right side if Width>0
  difference {
    torus {Bigrad + Thick - Tinyrad, Tinyrad rotate 90*x}
    Topclipper
    Botclipper
  }
  // Left side
  difference {
    torus {Bigrad + Thick - Tinyrad, Tinyrad rotate 90*x + _Width*y }
    Topclipper
    Botclipper
  }
  // Four balls in the corners
  sphere {(Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Lattop*-z}
  sphere {(Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Latbot*-z}
  sphere {(Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Lattop*-z rotate _Width * y}
  sphere {(Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Latbot*-z rotate _Width * y}
  
  // Now it looks a bit like a table, with the top overhanging the sides.

  difference
  {
    sphere {0, Bigrad + Thick - Tinyrad}
    sphere {0, Bigrad}
    Lefterclipper
    Righterclipper
    Topclipper
    Botclipper
  }
  // Now just overhanging top and bottom.
  difference
  {
    sphere {0, Bigrad + Thick - Tinyrad}
    sphere {0, Bigrad}
    Leftclipper
    Rightclipper
    Topperclipper
    Botterclipper
  }
  // Now there are square cutouts at the corners to fill.
  
  #if (0)
    // These rounded corners have bigger radii at the outside of the plate
    cone {0, 0, (Bigrad + Thick - Tinyrad) * y, Tinyrad  rotate Lattop*-z}
    cone {0, 0, (Bigrad + Thick - Tinyrad) * y, Tinyrad  rotate Latbot*-z}
    cone {0, 0, (Bigrad + Thick - Tinyrad) * y, Tinyrad  rotate Lattop*-z rotate _Width * y}
    cone {0, 0, (Bigrad + Thick - Tinyrad) * y, Tinyrad  rotate Latbot*-z rotate _Width * y}
  #else
    cylinder {0, (Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Lattop*-z} 
    cylinder {0, (Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Latbot*-z}
    cylinder {0, (Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Lattop*-z rotate _Width * y}
    cylinder {0, (Bigrad + Thick - Tinyrad) * y, Tinyrad rotate Latbot*-z rotate _Width * y}
  #end    
  

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

