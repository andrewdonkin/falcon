#include "shapes3.inc"

// This file defines macros that create and join segments of pipe to form plumbing.
// Each macro call creates a new segment and joins to the end of the previous.
// Segments are straight (plumb_tube*) or curved (plumb_left/right).
// At first the pipe can only wind around on the Y plane,
//  but you can change the notion of "up" for the "left/right" macros with plumb_twist.
// The radius is carried in a variable to save passing it to each macro,
//  but you can change it with "thinner" and "radstep*"

// Start an array of [n] pipes, each of radius [rad]
#macro plumb_start_n(n, rad)
  #declare plumb_moves_accum = array[n];
  #declare plumb_rad = array[n];
  #for (i, 0, n-1)
    #declare plumb_rad[i] = rad;
    #declare plumb_moves_accum[i] = transform {};
  #end
#end

#macro plumb_transform_n(n, xform)
  #declare plumb_moves_accum[n] = transform { xform plumb_moves_accum[n]}
#end

#macro plumb_tube_n(n, zed)
  cylinder {0, zed*z, plumb_rad[n] transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

#macro plumb_thinner_n(n, zed, radto)
  cone {0, plumb_rad[n], zed*z, radto transform plumb_moves_accum[n]}
  #declare plumb_rad[n] = radto;
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

// +ve means anticlockwise, if the rest of the tube is behind us
#macro plumb_twist_n(n, ang)
  #declare plumb_moves_accum[n] = transform { rotate ang*z plumb_moves_accum[n]}
#end  

#macro plumb_left_n(n, major, ang)
  // Segment_of_Torus:  starts at x=major, y=z=0, goes toward +z (anticlockwise) with -ang < 0
  object {Segment_of_Torus (major, plumb_rad[n], -ang) translate <-major, 0, 0> transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform {
    translate <major, 0, 0> rotate <0, -ang, 0> translate <-major, 0, 0>
    plumb_moves_accum[n]
  }
#end

#macro plumb_right_n(n, major, ang)
  object { Segment_of_Torus (major, plumb_rad[n], -ang) translate <-major, 0, 0> scale <-1, 1, 1>  transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform {
    translate <-major, 0, 0> rotate <0, ang, 0> translate <major, 0, 0>
    plumb_moves_accum[n]
  }
#end

#macro plumb_radstep_n(n, radto, zed, muffler_p)
  #local Bigrad = max(plumb_rad[n], radto);
  #if (zed = 0)
    #local Minorrad = Bigrad / 10;
  #else
    #local Minorrad = zed;
  #end
  torus {Bigrad - Minorrad, Minorrad rotate 90*x
    transform plumb_moves_accum[n]
  }
  #if (muffler_p = 0)
    // draw a disc on the smaller-radius side of the torus so that it forms
    // a curved edge to the bigger cylinder.
    // Otherwise the sides of a cylinder roll over onto the ends, like an old muffler.
    disc {0, z, Bigrad-Minorrad
      translate sgn(plumb_rad[n] - radto) * Minorrad*z
      transform plumb_moves_accum[n]
    }
  #end
  #declare plumb_rad[n] = radto;
#end


//---------



#macro plumb_start(rad) plumb_start_n(1, rad) #end

#macro plumb_transform() plumb_moves_accum[0] #end

#macro plumb_tube(zed) plumb_tube_n(0, zed) #end

#macro plumb_thinner(zed, radto) plumb_thinner_n(0, zed, radto) #end

#macro plumb_twist(ang) plumb_twist_n(0, ang) #end

#macro plumb_left(major, ang) plumb_left_n(0, major, ang) #end

#macro plumb_right(major, ang) plumb_right_n(0, major, ang) #end

