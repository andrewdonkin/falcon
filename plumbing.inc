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

// Directly set the transformation required to get to this part of the pipe.
// Generally used for parts 2..n of a gang.
#macro plumb_transform_n(n, xform)
  #declare plumb_moves_accum[n] = transform { xform plumb_moves_accum[n]}
#end

// Straight tube, no change in radius, on vector z.
#macro plumb_tube_n(n, zed)
  cylinder {0, zed*z, plumb_rad[n] transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

// Cone, changing pipe from one radius to another over a distance zed.
// You know where you can stick your complaint about the name.
#macro plumb_thinner_n(n, radto, zed)
  cone {0, plumb_rad[n], zed*z, radto transform plumb_moves_accum[n]}
  #declare plumb_rad[n] = radto;
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

// +ve means anticlockwise, if the rest of the tube is behind us.
// Point your left thumb in the direction of the pipe, fingers curl +ve.
#macro plumb_twist_n(n, ang)
  #declare plumb_moves_accum[n] = transform { rotate ang*z plumb_moves_accum[n]}
#end  

// turn left ang degrees around a major radius.
// If major==0, use a radius that translates forward the pipe radius in a 45 degree turn.
// In that way, a left and a right with major==0 moves the pipe forward double its radius.
// That makes it easier to compute the z-size of a pipe with a dog-leg in it.
#macro plumb_left_n(n, ang, major)
  #if (major=0) #local major=sqrt(2)*plumb_rad[n]; #end
  // Segment_of_Torus:  starts at x=major, y=z=0, goes toward +z (anticlockwise) with -ang < 0
  object {Segment_of_Torus (major, plumb_rad[n], -ang) translate <-major, 0, 0> transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform {
    translate <major, 0, 0> rotate <0, -ang, 0> translate <-major, 0, 0>
    plumb_moves_accum[n]
  }
#end

#macro plumb_right_n(n, ang, major)
  #if (major=0) #local major=sqrt(2)*plumb_rad[n]; #end
  object { Segment_of_Torus (major, plumb_rad[n], -ang) translate <-major, 0, 0> scale <-1, 1, 1>  transform plumb_moves_accum[n]}
  #declare plumb_moves_accum[n] = transform {
    translate <-major, 0, 0> rotate <0, ang, 0> translate <major, 0, 0>
    plumb_moves_accum[n]
  }
#end

// plumb_radstep_n changes pipe radius without adding any length.
// A torus that smooths the transition extends into the smaller pipe's space
// (and into the larger pipe's space equally, but inside that pipe).
#macro plumb_radstep_n(n, radto, minor_rad, muffler_p)
// radto is the radius to change to
// minor_rad is the radius of the smoothing on the rim of the bigger pipe/
//  If zero, pick a sensible default.
// muffler_p is a string.  If not "y", draw a disc on the smaller-radius side
//  of the smoothing torus so that it forms a curved edge to the bigger pipe.
//  Otherwise, the sides of a cylinder roll over onto the ends, like an old muffler.
  #local Bigrad = max(plumb_rad[n], radto);
  #if (minor_rad = 0)
    #if (muffler_p = "y")
      #local Minorrad = Bigrad / 10;
    #else
      #local Minorrad = Bigrad / 20;
    #end
  #else
    #local Minorrad = minor_rad;
  #end
  torus {Bigrad - Minorrad, Minorrad rotate 90*x
    transform plumb_moves_accum[n]
  }
  #if (muffler_p != "y")
    disc {0, z, Bigrad-Minorrad
      translate sgn(plumb_rad[n] - radto) * Minorrad*z
      transform plumb_moves_accum[n]
    }
  #end
  #declare plumb_rad[n] = radto;
#end


#macro plumb_splines_n(n, num_splines, zed, ang_offset)
// Not what you think.  More like splines on an drive shaft.
  #for (i, 0, num_splines-1)
    box {0, <plumb_rad[n]/5, plumb_rad[n]/2, zed>
         translate <0, plumb_rad[n], 0>
         rotate ((360 / num_splines * i) + ang_offset) * z
         transform plumb_moves_accum[n]
         }
  #end
#end


//---------



#macro plumb_start(rad) plumb_start_n(1, rad) #end

#macro plumb_transform() plumb_moves_accum[0] #end

#macro plumb_tube(zed) plumb_tube_n(0, zed) #end

#macro plumb_thinner(zed, radto) plumb_thinner_n(0, zed, radto) #end

#macro plumb_twist(ang) plumb_twist_n(0, ang) #end

#macro plumb_left(major, ang) plumb_left_n(0, major, ang) #end

#macro plumb_right(major, ang) plumb_right_n(0, major, ang) #end

