#include "shapes3.inc"
#declare pseed = seed(1);


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


#macro plumb_splines_n(n, zed, num_splines, radratio, ang_offset)
  // Not what you think.  More like splines on a drive shaft.
  // Does not shift the plumbing at all - just adds splines around it.
  difference{
    union {
      #for (i, 0, num_splines-1)
      object {
        Segment_of_CylinderRing(plumb_rad[n]*radratio, 0, zed, 360/num_splines/2)
        rotate <90, 0, 0>
        translate <-plumb_rad[n], 0, 0> rotate <rand(pseed)*2, rand(pseed)*2, 0> translate <plumb_rad[n], 0, 0>
        #if (0)
          box {0, <plumb_rad[n]/5, plumb_rad[n]/2, zed>
            translate <0, plumb_rad[n], 0>}
        #end
        rotate ((360 / num_splines * i) + ang_offset) * z
      }
#end
    } // union
    // subtract away the interior, otherwise it looks like a sunburst
    cylinder{<0, 0, -zed*1.1>, <0, 0, zed*1.1>, plumb_rad[n]*0.8}
    transform plumb_moves_accum[n]
  }
#end

// Ribs in the pipe.  Increasing from the radius, like it's been inflated.
// Each rib swells out to radius + radius*bumpratio
#macro plumb_ribs_out_n(n, zed, num_ribs, bumpratio)
  #if (num_ribs < 1)
    # error concat("I need ribs.  n:", str(n,0,0),
    " zed:", str(zed,0,0),
      " num_ribs:", str(num_ribs,0,0),
      " bumpratio:", str(bumpratio,0,0))
  #end
  isosurface {
    function { sqrt(y*y+x*x)  // radius of the rendered pipe...
      + cos(z/zed*num_ribs*2*pi) * plumb_rad[n] * bumpratio/2 // ...plus cosine
      - plumb_rad[n]*(1+bumpratio/2) }  // ...equals expanded radius
    contained_by {box {<-plumb_rad[n]*(1.1+bumpratio), -plumb_rad[n]*(1.1+bumpratio), 0>,
      <plumb_rad[n]*(1.1+bumpratio), plumb_rad[n]*(1.1+bumpratio), zed>}}
    accuracy 0.005 * plumb_rad[n]
    //max_gradient 0.06 // For bumpratio 1, 1+0.6*num_ribs-1
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

// Ribs in the pipe.  Reducing the radius, like it's been gripped.
// Each corrugation goes in to radius - radius*bumpratio
#macro plumb_ribs_in_n(n, zed, num_ribs, bumpratio)
  isosurface {
    function { sqrt(y*y+x*x)
      - cos(z/zed*num_ribs*2*pi)*plumb_rad[n]*bumpratio/2
      - plumb_rad[n]*(1-bumpratio/2) }
    // expand the container to fit inaccuracies
    contained_by {box {<-plumb_rad[n]*1.01, -plumb_rad[n]*1.01, 0>,
      <plumb_rad[n]*1.01, plumb_rad[n]*1.01, zed>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

//#macro plumb_spiral_para_n(n, zed, num_turns, major)
//parametric{
//  function{ plumb_rad[n] * cos(u) + major * cos(v/zed*num_turns*2*pi) } // x(u,v)
//  function{ plumb_rad[n] * sin(u) + major * sin(v/zed*num_turns*2*pi) }   // y(u,v)
//  function{ v }        // z(u,v)
//  <0,0>, <2*pi, zed>  // start, end(u,v)
//  contained_by {box {<-plumb_rad[n] - major*1.2, -plumb_rad[n] - major*1.2, 0>,
//                     <plumb_rad[n] + major *1.2, plumb_rad[n] + major *1.1, zed>}}
//  max_gradient 0.00000001 ///...seems to work
//  accuracy 0.001 // * plumb_rad[n] ?
//  precompute 20 x,y,z
//  transform plumb_moves_accum[n]
//}
//#declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
//#end
//
//#macro plumb_spiral_builtin_n(n, zed, num_turns, major)
//isosurface {
//  function { 
//    f_helix1( x,y,z,
//        1,    // number of helixes, (1 = single helix, 2 = double helix etc.)
//        num_turns*2*pi,// For number N of turns per heigth of y = 1 unit, use N* 2*pi
//        plumb_rad[n], // minor radius,
//        major, // major radius,
//        1,    // shape parameter,
//        1,  // cross section type,
//              // 0.0 to 1.0 = square ... rounded to circle
//              // over 2.0 to 3.0 = rounded to diamond and concave diamond
//        0 )   // cross section rotation angle
//  }
//  contained_by {box {<-major-plumb_rad[n], 0, -major-plumb_rad[n]>,
//                       <major+plumb_rad[n], zed, major+plumb_rad[n]>}}
//  evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
//  rotate 90*x
//  transform plumb_moves_accum[n]
//}
//#declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
//#end

// Starts at x=majorx, y=0, so you need the leadin and leadout macros
// to get the pipe back to x=0
#macro plumb_spiral_n(n, zed, num_turns, majorx, majory)
  #local plumbf=function (x,y,x1,y1) {
    sqrt(pow(x-x1, 2) + pow(y-y1, 2))
    - plumb_rad[n]
  };
  isosurface {
    function { plumbf(x,y,
      cos(z/zed*num_turns*2*pi) * majorx,
      sin(z/zed*num_turns*2*pi) * majory) }
    contained_by {box {<-majorx-plumb_rad[n], -majory-plumb_rad[n], 0>,
      <majorx+plumb_rad[n], majory+plumb_rad[n], zed>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001 * plumb_rad[n] // default is fine until you stitch them together
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]};
#end

// soft joiner from x=0 out to x=majorx
#macro plumb_spiral_start_n(n, zed, majorx)
  isosurface {
    function { sqrt(pow(x - (- cos(z/zed*pi) * majorx/2 + majorx/2), 2) + y*y)
      - plumb_rad[n]}
    contained_by {box {<-plumb_rad[n], -plumb_rad[n], 0>,
      <majorx+plumb_rad[n], plumb_rad[n], zed>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001 * plumb_rad[n]
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

// soft joiner from x=majorx back to x=0.  This one only moves in the X plane
// and does not twist + or - y, so it does not lead well into actual spirals.
// Fooled you with the name.  Snowcones are better.
#macro plumb_spiral_finish_n(n, zed, majorx)
  isosurface {
    function { sqrt(pow(x - (  cos(z/zed*pi) * majorx/2 + majorx/2), 2) + y*y)
      - plumb_rad[n]}
    contained_by {box {<-plumb_rad[n], -plumb_rad[n], 0>,
      <majorx+plumb_rad[n], plumb_rad[n], zed>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001 * plumb_rad[n]
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

//####################################################################
// Returns [0,1] for input [0,1].  Returns 0.5 for input 0.5.
// But it is an S-shape in between.
// I use it in the spirals to avoid sudden gradient changes at the ends.
#declare softly = function(in) {
  select(in-0.5,
    0.5*(1-cosd(90*in))/(1-sind(45)),                            // in < 0.5
    0.5+0.5*(sind(90*(min(in,1))) - sind(45)) / (1-sind(45))      // in >= 0.5
  )
}
// Another spiral, that starts with radius zero, so you don't need to ease into it.
// If the final parameter "mirrorz" is true, starts at full radius and dimishes to zero.
// Use snowcone and snowcone_out instead of this.
// Keep the pitch the same through all three macros:  snowcone, spiral, and snowcone_out.
// Easiest way to do that is with a wire_handle.
#macro plumb_wirehandle_n(n, zed, in_turns, const_turns, out_turns, majorx, majory)
  #local num_turns = in_turns + const_turns + out_turns;
  #if (in_turns)
    plumb_snowcone_start_n(n, zed * in_turns / num_turns, in_turns, majorx, majory)
  #else
    #warning "wire handles are disconnected without an opening snowcone.  Hope that makes sense."
  #end
  #if (const_turns)
    plumb_spiral_n(n, zed * const_turns / num_turns, const_turns, majorx, majory)
  #end
  #if (out_turns)
    plumb_snowcone_finish_n(n, zed * out_turns / num_turns, out_turns, majorx, majory)
  #else
    #warning "wire handles are disconnected without a closing snowcone."
  #end
#end

// Spiral out from the centre to x=major
#macro plumb_snowcone_start_n(n, zed, num_turns, majorx, majory)
  _plumb_snowcone_n(n, zed, num_turns, majorx, majory, 0)
#end
// Spiral in from x=major to the centre
#macro plumb_snowcone_finish_n(n, zed, num_turns, majorx, majory)
  _plumb_snowcone_n(n, zed, num_turns, majorx, majory, 1)
#end

#macro _plumb_snowcone_n(n, zed, num_turns, majorx, majory, mirrorz)
  #local plumbf=function (x,y,x1,y1) {
    sqrt(pow(x-x1, 2) + pow(y-y1, 2))
    - plumb_rad[n]}
  isosurface {
    function { plumbf(x,y,
      cos(z/zed*num_turns*2*pi) * majorx * softly(
        #if (mirrorz) 1- #end z/zed),
      sin(z/zed*num_turns*2*pi) * majory * softly(
        #if (mirrorz) 1- #end z/zed)) }
    contained_by {box {<-majorx-plumb_rad[n], -majory-plumb_rad[n], 0>,
      <majorx+plumb_rad[n], majory+plumb_rad[n], zed>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001 * plumb_rad[n] // default is fine until you stitch them together
    transform plumb_moves_accum[n]
  }
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end

//---------

#macro plumb_ssweep_n(n, zed, num_turns, majorx, majory)
  #local r = plumb_rad[n];
  #local divs=26;
  #local gentle = 0.1;
  sphere_sweep {
    cubic_spline
    num_turns * divs + 4, // start and end points plus control points
    <0, 0, -r>, r  // first control point, doesn't matter, someone on -Z
    <0, 0, 0>, r 
    
    #local h=0;  #while (h < num_turns * divs)
      #local i = h / divs;
      #switch (i / num_turns)
        #range (0, gentle)
          #local rratio = softly(i / num_turns / gentle);
        #break;
        #range (1-gentle, 1)
          #local rratio = softly((1 - (i / num_turns )) / gentle);
        #break;
      #else
        #local rratio = 1;
      #end
      <majorx * cos(i*2*pi) * rratio,
      majory * sin(i*2*pi) * rratio,
      i * zed / num_turns>, r
      #local h=h+1;
    #end
    
    <0, 0, zed>, r
    <0, 0, zed + r>, r // last control point
    transform plumb_moves_accum[n]
  }
    
  #declare plumb_moves_accum[n] = transform { translate zed*z plumb_moves_accum[n]}
#end
//---------

// Enter the marshmallow.  See how one of the pipes ends in BottomFront-6.jpg.  It's like
// a squashed ball with a cavity toward -Z ready to receive a pipe.  Terminate a radius
// 1.0 pipe in it at y=1.5 relative to its origin.
#declare plumb_marshmallow =
  intersection{
    blob {
      threshold 0.01
      sphere { <0,1.5,0>, 6, 1 }
      cylinder {<0, 4,-3>, <0, 4 ,-4>, 3, -0.8}
      scale <1, 0.5, 1>
    }
    box {<-6, 0, -6>, <6,4,6>}
  };
  
//---------

#macro plumb_start(rad) plumb_start_n(1, rad) #end

#macro plumb_transform() plumb_moves_accum[0] #end

#macro plumb_tube(zed) plumb_tube_n(0, zed) #end

#macro plumb_thinner(zed, radto) plumb_thinner_n(0, zed, radto) #end

#macro plumb_twist(ang) plumb_twist_n(0, ang) #end

#macro plumb_left(major, ang) plumb_left_n(0, major, ang) #end

#macro plumb_right(major, ang) plumb_right_n(0, major, ang) #end

