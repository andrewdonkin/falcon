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

  #local toeinr=radians(toein);
  #local casterfrontr=radians(casterfront);
  #local camberr=radians(camber);

  union {

  intersection {
    plane { -z, 0 rotate casterfront * x} // front
    plane { z, 0 rotate -casterback * x translate maxz * z } // back
    plane { x, 0 rotate camber * z rotate -toein * y translate maxx * x} // right
    plane { -x, 0 rotate -camber * z rotate toein * y translate -maxx * x} // left
    plane { -y, 0} // bottom
    plane { y, 0 rotate topslope* x translate <0, maxy, tan(casterfrontr)*maxy>} // top
  hollow
  texture {pigment {rgbf <1, 1, 1, 0.6>} }  
  interior { ior 1}
    }
  
  union {
  // debugging rods
  //cylinder { -maxx*x, maxx*x, smallr/10}
  // cylinder { <-maxx, maxy, 0> <maxx, maxy, 0>, smallr/20}
  //cylinder { <-maxx, maxy, maxz> <maxx, maxy, maxz> smallr/4}
  //cylinder { <-maxx, 0, maxz> <maxx, 0, maxz> smallr/4}
  
  #local blf = <-maxx, 0, 0>;
  #local brf = <maxx, 0, 0>;
  #local blr = <(tan(toeinr) * maxz)-maxx, 0, maxz>;
  #local brr = blr * <-1, 1, 1>;
  
  #local tlf = <tan(camberr)*maxy, maxy, tan(casterfrontr)*maxy>; // this is before toe-in, if blf was origin
  #local tlf2 = vrotate(tlf, toein * y);  // rotated it clockwise (looking down) by toe-in.  tlf2.z <= tlf.z
  // Interested in how far its Z coord moved
  #local tlf = <tlf2.x + (tlf.z - tlf2.z) * tan(toeinr) - maxx, maxy, tlf.z>;
  #local trf = tlf * <-1, 1, 1>;

  cylinder { tlf, trf, smallr/20}

  // top rear is harder.

//  #local sidenormal = vrotate(vrotate(x, <0, 0, camber>), <0, -toein, 0>);


  #local Green = <trf.x - (maxz - trf.z) * tan(toeinr), maxy, maxz>;
  cylinder {Green, brr, smallr/400 pigment {rgb <0, 1, 0>}}
  // Green == trr if casterback == topslope == 0
  // Push the top rear downward
  #if (topslope > 0)
    #local Yellow  = vrotate(Green - trf, x * topslope);
    // lengthen it onto Z=maxz and make absolute
    #local Yellow  = Yellow * ((Green.z -trf.z) / Yellow.z) + trf;
    // Now z and y are correct, but it is left of where it should be (and still no casterback)
    // make it relative to Green, account for camber by projecting onto Green->brr
    #local Cyann = VProject_Axis(Yellow-Green, brr - Green);
    // high.  Push it down, then make absolute
    #local Cyann  = Cyann * (Green.y-Yellow.y) / -Cyann.y + Green;
    cylinder {Cyann, trf, smallr/200 pigment {rgb <0, 1, 1>}}
  #else
    #local Yellow = Green;
    #local Cyann = Green;
  #end
    
  // Cyann == trr if casterback == 0.
  // The vector trr->Cyan runs perfectly along the TR edge,
  // but if casterback > 0 the vector is too long
  // (Cyann is too +z, and too +x if there is toe in, and too +y if there is topslope)
 
  #if (casterback > 0)
    // Now add casterback:  pull the top rear forward

    #local backnormal = vrotate(z, <-casterback, 0, 0>);

 //   #local Purple  = brr + vrotate(Cyann - brr, -x * casterback);
    #local Purple  = VProject_Plane(Cyann - brr, backnormal) + brr;
    // Has gone low. Project it back up onto trf->Yellow
//   #local trr = VProject_Axis(Purple - Cyann, trf - Cyann) + Cyann;
    #local trr = <Purple.x, trf.y - (trf.y-Cyann.y) * (Purple.z-trf.z) / (Cyann.z-trf.z), Purple.z>;
  #else
    #local Purple  = Cyann;
    #local trr = Cyann;
  #end


  sphere {Green, smallr/30 pigment {rgb <0, 1, 0>}}

  union {sphere {Yellow, smallr/30 } cylinder {Yellow, Yellow+y*0.1, smallr/200} pigment {rgb <1, 1, 0>}}
  cylinder {Yellow, trf, smallr/200 pigment {rgb <1, 1, 0>}}
  union {sphere {0, smallr/30 } cylinder {0, y*0.05, smallr/200} cylinder {0, -z*0.05, smallr/200} translate Cyann pigment {rgb <0, 1, 1>}}
  union {sphere {0, smallr/30 } cylinder {0, -x*0.05, smallr/200} cylinder {0, y*0.07, smallr/200} 
         translate Purple pigment { rgb <1, 0, 1>}}
  cylinder {brr, brr + (Purple-brr)*1.1, smallr/200}
  union {sphere {0, smallr/30} cylinder {0, -z*0.05, smallr/200}
         cylinder {0, -x*0.05, smallr/200}
         cylinder {0, -y*0.05, smallr/200}
         translate trr pigment {rgb <1, 0, 0>}}
  
  #local tlr = trr * <-1, 1, 1>;
  
  
  sphere {blf, smallr/4}
  sphere {brf, smallr/4}
  sphere {blr, smallr/4}
  sphere {brr, smallr/4}

  sphere {tlf, smallr/4}
  sphere {trf, smallr/4}

//  sphere {trr, smallr/30}
//  sphere {tlr, smallr/30}

//  sphere {trr, smallr/4}


    pigment {rgb x}
  }

  }
  
#end
  