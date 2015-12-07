// A pollywantahedron is a hexdron (cuboid, non-rectangular sides)
// with extra constraints and an easier interface.

// A hexdron is a polyhedron with six quadrilateral sides, not necessarily
// rectangles, which is the definition of "cuboid" according to some.
// With the edges rounded off.  Parameters are the vertices of the corners.
// That allows for non-flat sides, which are probably not desirable.
// Aside:  a saddle would handle non-flat sides better.  See
// http://www.geocities.ws/evilsnack/matrix.html .
// Picking the vertices is a p.i.t.a.
// Can't remember why I called it a hexdron.

#include "math.inc" // VAngleD

#macro hexdron(
  tlf, tlr, trr, trf, // top quadrilateral.  Top left front, top left rear, etc.
  blf, blr, brr, brf, // bottom quadrilateral
  smallr	      // radius of rounded edges
  )
  
  #if (0)   // assume our shape is uninteresting enough that we can do this:
    #local tlf = tlf + < smallr, -smallr, smallr>;
    #local tlr = tlr + < smallr, -smallr,-smallr>;
    #local trr = trr + <-smallr, -smallr,-smallr>;
    #local trf = trf + <-smallr, -smallr, smallr>;
    #local blf = blf + < smallr,  smallr, smallr>;
    #local blr = blr + < smallr,  smallr,-smallr>;
    #local brr = brr + <-smallr,  smallr,-smallr>;
    #local brf = brf + <-smallr,  smallr, smallr>;
  #end
  
  #if (vlength(vnormalize(vcross(tlr-tlf, trf-tlf))
    - vnormalize(vcross(trf-trr, tlr-trr)) ))
    #warning "not a flat top quadrilateral"
  #end
  #if (vlength(vnormalize(vcross(blr-blf, brf-blf))
    - vnormalize(vcross(brf-brr, blr-brr)) ))
    #warning "not a flat bottom quadrilateral"
  #end
  // should do the other four too
  
  // Draw a quadrilateral between four points.  If not on a plane, it will miss one of the four.
  #macro hexface (
    lf, lr, rr, rf    // rectangle
    )
    // use smallr from parent scope, ewww
    #local nml = smallr * vnormalize(vcross(lr-lf, rf-lf));
    
    triangle {lf+nml, lr+nml, rr+nml}
    triangle {rr+nml, rf+nml, lf+nml}
  #end

  union {
    hexface(tlf, tlr, trr, trf) // top
    hexface(blf, brf, brr, blr) // bottom, clockwise
    hexface(blf, tlf, trf, brf) // front
    hexface(blr, tlr, tlf, blf) // left
    hexface(brr, trr, tlr, blr) // rear, still going clockwise from apparant bot left
    hexface(brf, trf, trr, brr) // right

    #if (1)
      sphere {tlf, smallr}  sphere {tlr, smallr}  sphere {trr, smallr}  sphere {trf, smallr}
      sphere {blf, smallr}  sphere {blr, smallr}  sphere {brr, smallr}  sphere {brf, smallr}
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
#macro pollywantahedron (
  maxx, maxy, maxz, // gross dimensions, bounding box
  toein, camber, casterfront, casterback, topslope// angles away from a rectangular cuboid
  smallr // radius of rounded edges
  )

  object {
  #local toeinr=radians(toein);
  #local casterfrontr=radians(casterfront);
  #local camberr=radians(camber);


  #local blf = <-maxx, 0, 0>; // bottom left front
  #local brf = <maxx, 0, 0>;  // bottom right front
  #local blr = <(tan(toeinr) * maxz)-maxx, 0, maxz>; // bottom left rear.  Moved right by toe in.
  #local brr = blr * <-1, 1, 1>; // bottom right rear
  
  // Top left front.  Moved right by camber, and +Z by front caster, then right and -Z by toe in
  #local tlf = <tan(camberr)*maxy, maxy, tan(casterfrontr)*maxy>; // this is before toe-in, and relative to blf
  #local tlf2 = vrotate(tlf, toein * y);  // rotated it clockwise (looking down) by toe-in.  tlf2.z <= tlf.z
  // Interested in how far its Z coord moved
  #local tlf = <tlf2.x + (tlf.z - tlf2.z) * tan(toeinr) - maxx, maxy, tlf.z>;
  #local trf = tlf * <-1, 1, 1>;

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

  #local tlr = trr * <-1, 1, 1>;

  #if (0)
  union {

  intersection {
    plane { -z, 0 rotate casterfront * x} // front
    plane { z, 0 rotate -casterback * x translate maxz * z } // back
    plane { x, 0 rotate camber * z rotate -toein * y translate maxx * x} // right
    plane { -x, 0 rotate -camber * z rotate toein * y translate -maxx * x} // left
    plane { -y, 0} // bottom
    plane { y, 0 rotate topslope* x translate <0, maxy, tan(casterfrontr)*maxy>} // top
    //hollow texture {pigment {rgbf <1, 1, 1, 0.6>} } interior { ior 1}
    }
  }
  #else
    hexdron(
      tlf, tlr, trr, trf,
      blf, blr, brr, brf,
      smallr
    )
  #end
  
  
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

  }
  
#end
  