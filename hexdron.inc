// A pollywantahedron is a hexdron (cuboid, non-rectangular sides)
// with extra constraints and an easier interface.
// Parameters maxx, maxy, maxz (dimensions)
//            toein, camber, casterfront, casterback, topslope (angles)
// You are standing at a -Z looking +Z.  Imagine two parallel planes
// on X=0 and X=maxx.  Tilt them both inward by camber degrees,
// so their tops meet at an angle of 2*camber degrees along a line
// parallel to the Z axis on x=maxx/2, y=some positive constant.
// Their bottoms are still on x=0,maxx y=0.
// Now yaw the +Z extents toward each other by toein degrees, pivoting
// where they touch the Z axis at X=0 and maxx, Y=0, Z=0.
// Now they meet along another line at some +Z.  These are the sides.
// Add a front plane z=0.  Push its top away from you by casterfront degrees.
// Add a top plane Y=maxy.  Fix its front on the line where it intersects
// the front plane, and pitch it down at the back by topslope degrees.
// Add a back plane Z=maxz.  Fix its bottom at Y=0, pitch it toward you
// by casterback degrees.
// The bottom is the plane Y=0.
// The minimum bounding box is {0, <maxx, maxy, maxz>}.
// The last parameter is the radius to put on the edges.

// A hexdron is a polyhedron with six quadrilateral sides, not necessarily
// rectangles, which is the definition of a "cuboid" according to some.
// With the edges rounded off.  Parameters are the vertices of the corners.
// That allows for non-flat sides, which are probably not desirable
// but make a perfectly good dodecahedron.
// NOTE:  its extents are increased beyond the corner params by the edge radius!
// That saves a lot of geometry.
// Corners must be clockwise around the top,
// then clockwise (viewed from the top) around the bottom
// starting from the corner below the first.
// The "Use_saddle" must be false if you have any perfectly flat sides.
// Can't remember why I called it a hexdron.
// Six sides, I guess.  Or maybe I felt cursed, working on it?

// Aside:  a saddle would handle non-flat sides better.  See
// http://www.geocities.ws/evilsnack/matrix.html .
// Picking the vertices is a p.i.t.a.

// #include "math.inc" // VAngleD not needed any more.  DIY FTW?

#macro hexdron(
  tlf, tlr, trr, trf, // top quadrilateral.  Top left front, top left rear, etc.
  blf, blr, brr, brf, // bottom quadrilateral
  smallr	      // radius of rounded edges
  , Use_saddle
  )
  
  #if (vlength(vnormalize(vcross(tlr-tlf, trf-tlf))
    - vnormalize(vcross(trf-trr, tlr-trr)) ))
    #warning "not a flat top quadrilateral"
  #end
  #if (vlength(vnormalize(vcross(blr-blf, brf-blf))
    - vnormalize(vcross(brf-brr, blr-brr)) ))
    #warning "not a flat bottom quadrilateral"
  #end
  // should do the other four too
  // Turns out, thanks to extra effort below, it looks good provided the plane is concave,
  // so we needn't fuss.
  
  // Draw a quadrilateral between four points.
  #macro hexface (
    lf, lr, rr, rf    // rectangle
    )
    // use smallr and Use_saddle from parent scope, ewww
    #local nml_lf = smallr * vnormalize(vcross(lr-lf, rf-lf));
    #local nml_rr = smallr * vnormalize(vcross(rf-rr, lr-rr));
    #if (Use_saddle)
      #local nml_lr = smallr * vnormalize(vcross(rr-lr, lf-lr));
      #local nml_rf = smallr * vnormalize(vcross(lf-rf, rr-rf));
      saddleup (rf+nml_rf, lf+nml_lf, lr+nml_lr, rr+nml_rr)
    #else
      triangle {rf+nml_lf, lf+nml_lf, lr+nml_lf}
      triangle {lr+nml_rr, rr+nml_rr, rf+nml_rr}
    #end
  #end

  union {
    // faces
    hexface(tlf, tlr, trr, trf) // top
    hexface(blf, brf, brr, blr) // bottom, clockwise from apparent top left, no matter
    hexface(blf, tlf, trf, brf) // front
    hexface(blr, tlr, tlf, blf) // left
    hexface(brr, trr, tlr, blr) // rear, clockwise from apparent bot left
    hexface(brf, trf, trr, brr) // right

#if(Use_saddle)
#else
    // Diagonals between opposite corners on a face.
    // Unnecessary if faces are flat or concave.  Looks damned good if convex.
      cylinder {tlf, brf, smallr}  cylinder {tlr, blf, smallr} // front, left
      cylinder {trr, blr, smallr}  cylinder {trf, brr, smallr} // rear, right
      cylinder {tlr, trf, smallr}  cylinder {blr, brf, smallr} // rear, right
#end

    #if (1)
      // corners
      sphere {tlf, smallr}  sphere {tlr, smallr}  sphere {trr, smallr}  sphere {trf, smallr}
      sphere {blf, smallr}  sphere {blr, smallr}  sphere {brr, smallr}  sphere {brf, smallr}
      // edges
      cylinder {tlf, tlr, smallr}  cylinder {tlr, trr, smallr} // around the top
      cylinder {trr, trf, smallr}  cylinder {trf, tlf, smallr}
      cylinder {blf, blr, smallr}  cylinder {blr, brr, smallr} // around the bot
      cylinder {brr, brf, smallr}  cylinder {brf, blf, smallr}
      cylinder {tlf, blf, smallr}  cylinder {tlr, blr, smallr} // left verticals
      cylinder {trf, brf, smallr}  cylinder {trr, brr, smallr} // right verticals
    #end
  }
    
#end


//========================================
// http://www.geocities.ws/evilsnack/matrix.html
// clockwise or counter-clockwise order
// so that SaddleA and SaddleC are on opposite corners
#macro saddleup (SaddleA, SaddleB, SaddleC, SaddleD)

  #local SaddleX=(SaddleA-SaddleB-SaddleC+SaddleD)/4;
  #local SaddleY=(SaddleA+SaddleB-SaddleC-SaddleD)/4;
  #local SaddleZ=(SaddleA-SaddleB+SaddleC-SaddleD)/4;
  #local SaddleM=(SaddleA+SaddleB+SaddleC+SaddleD)/4;

  quadric { <0,0,0>,<1,0,0>,<0,0,-1>,0
    hollow
    clipped_by { box { -1,1 } }
    bounded_by { clipped_by }
    matrix < SaddleX.x,SaddleX.y,SaddleX.z,
           SaddleY.x,SaddleY.y,SaddleY.z,
           SaddleZ.x,SaddleZ.y,SaddleZ.z,
           SaddleM.x,SaddleM.y,SaddleM.z > }
#end

  
//========================================
#macro pollywantahedron (
  maxx, maxy, maxz, // gross dimensions, bounding box
  toein, camber, casterfront, casterback, topslope// angles away from a rectangular cuboid
  smallr // radius of rounded edges
  )

  #if (0)

  // simple version, no curved edges.  This is what I was aiming for below.
  union {

    intersection {
      plane { -z, 0 rotate casterfront * x} // front
      plane { z, 0 rotate -casterback * x translate maxz * z } // back
      plane { x, 0 rotate camber * z rotate -toein * y translate maxx * x} // right
      plane { -x, 0 rotate -camber * z rotate toein * y } // left
      plane { -y, 0} // bottom
      plane { y, 0 rotate topslope* x translate <0, maxy, tan(radians(casterfront))*maxy>} // top
      //hollow texture {pigment {rgbf <1, 1, 1, 0.6>} } interior { ior 1}
    }
  
    // union is still open

  #else

  #local toeinr=radians(toein);
  #local casterfrontr=radians(casterfront);
  #local camberr=radians(camber);


  #local blf = <0, 0, 0>; // bottom left front
  #local brf = <maxx, 0, 0>;  // bottom right front
  #local blr = <(tan(toeinr) * maxz), 0, maxz>; // bottom left rear.  Moved right by toe in.
  #local brr = <maxx-blr.x, 0, maxz>; // bottom right rear
  
  // Top left front.  Moved right by camber, and +Z by front caster, then right and -Z by toe in
  #local tlf = <tan(camberr)*maxy, maxy, tan(casterfrontr)*maxy>; // this is before toe-in, and relative to blf
  #local tlf2 = vrotate(tlf, toein * y);  // rotated it clockwise (looking down) by toe-in.  tlf2.z <= tlf.z
  // Interested in how far its Z coord moved
  #local tlf = <tlf2.x + (tlf.z - tlf2.z) * tan(toeinr), maxy, tlf.z>;
  #local trf = <maxx-tlf.x, maxy, tlf.z>;

  // top rear is harder.

  // imagine orthogonal view from right (looking toward -X)
  // vector from top left to bottom right is the base of a triangle with trr at its apex.
  #local basevector = <0, -maxy, maxz-trf.z>;
  #local baseslope = degrees(atan2(maxy, maxz-trf.z));
  #local tlangle = baseslope - topslope; // top left, when viewed from high X
  #local brangle = (90-baseslope) - casterback; // bottom right when viewed from high X
  #local tans = tan(radians(brangle))/tan(radians(tlangle));
  #local D1 = vlength(basevector) * tans / (1 + tans); // trust me
  #local Hite = D1 * tan(radians(tlangle));
  // if we moved D1 (length) down basevector then turned 90 deg left
  // then moved Hite forward we'd be at brr, in the orthogonal view.
  // basevector has constant X.  Find a vector at right angles to it.
  #local supertop = vcross(basevector, x);  // will also have constant X
  #local Green = <maxx, maxy, trf.z> + D1 * vnormalize(basevector) + Hite * vnormalize(supertop);
  // now Green.y and Green.z are the same as trr's.  Need X.
  #local trr = <trf.x
                - (Green.z-trf.z) * tan(radians(toein)) // move left for toe in
                + (maxy-Green.y) * tan(radians(camber)), // move right for camber
                Green.y, Green.z>;

//  #local sidenormal = vrotate(vrotate(x, <0, 0, camber>), <0, -toein, 0>);

  #local tlr = <maxx-trr.x, trr.y, trr.z>;

  object {
      hexdron(
      tlf, tlr, trr, trf,
      blf, blr, brr, brf,
      smallr
      , 0
    )
    // object is still open
  #end // the cheap way, or the hexdron way.
    
  #if (0)
    union {
    // debugging rods
    //cylinder { -maxx*x, maxx*x, smallr/10}
    // cylinder { <-maxx, maxy, 0> <maxx, maxy, 0>, smallr/20}
    //cylinder { <-maxx, maxy, maxz> <maxx, maxy, maxz> smallr/4}
    //cylinder { <-maxx, 0, maxz> <maxx, 0, maxz> smallr/4}
    
  
    cylinder { tlf, trf, smallr/20}
  
    #warning concat("baseslope ", str(baseslope, 1, 0),
                    " tlangle ", str(tlangle, 1, 0),
                    " brangle ", str(brangle, 1, 0),
                    " D1 ", str(D1, 1, 3),
                    " supertop.y ", str(supertop.y, 1, 3),
                    " Green ", str(Green.y, 1, 3), ",", str(Green.z, 1, 3)
                    )
    cylinder {<maxx, maxy, trf.z>, <maxx, 0, maxz>, smallr/100 pigment {rgb <0, 1, 0>}}
    
    union {sphere {0, smallr/10 } cylinder {0, -x*0.5, smallr/200} cylinder {0, y*0.05, smallr/200}
      translate Green pigment { rgb <0, 1, 0>}}
    
    sphere {blf, smallr/4}
    sphere {brf, smallr/4}
    sphere {blr, smallr/4}
    sphere {brr, smallr/4}
  
    sphere {tlf, smallr/4}
    sphere {trf, smallr/4}
  
    sphere {trr, smallr/30}
  //  sphere {tlr, smallr/30}
  
      pigment {rgb x}
    }
  #end // debug #if

  } // end hexdron object (normally), or intersection of planes
  
#end
