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
// With the edges rounded off.  Parameters are the vertices of the corners,
// before they got sanded off.
// That allows for non-flat sides, which makes a perfectly good dodecahedron
// with triangular sides.
// The resulting shape should not extend beyond the corner params.
// If your corners are tight, it may be considerably smaller.
// Corners must be clockwise around the top,
// then clockwise (viewed from the top) around the bottom
// starting from the corner below the first.
// The "Use_saddle" must be false if you have any flat sides.
// Can't remember why I called it a hexdron.
// Six sides, I guess.  Or maybe I felt cursed, working on it?

// Aside:  a saddle handles non-flat sides better.  See
// http://www.geocities.ws/evilsnack/matrix.html .
// Picking the vertices is a p.i.t.a.

// #include "math.inc" // VAngleD not needed any more.  DIY FTW?
#include "stdafx.inc"

#macro hexdron_saddle(TLF, TLR, TRR, TRF, BLF, BLR, BRR, BRF, Smallr)
  _hexdron(TLF, TLR, TRR, TRF, BLF, BLR, BRR, BRF, Smallr
    , 1        // Use_saddle
  )
#end

#macro hexdron(TLF, TLR, TRR, TRF, BLF, BLR, BRR, BRF, Smallr)
  _hexdron(TLF, TLR, TRR, TRF, BLF, BLR, BRR, BRF, Smallr
    , 0        // Use_saddle
  )
#end

#macro _hexdron(
  TLF, TLR, TRR, TRF, // top quadrilateral.  Top left front, top left rear, etc.
  BLF, BLR, BRR, BRF, // bottom quadrilateral
  Smallr	          // radius of rounded edges
  , optional Use_saddle        // doesn't work if a side is flat
  )
  
  #ifndef (Use_saddle)
  # local Use_saddle = 0;
  #end
  #ifdef (CHEAP)
  # local Smallr = 0;
  #end
  
  //#if (vlength(vnormalize(vcross(tlr-tlf, trf-tlf))
  //  - vnormalize(vcross(trf-trr, tlr-trr)) ))
  //  #warning "not a flat top quadrilateral"
  //#end
  //#if (vlength(vnormalize(vcross(blr-blf, brf-blf))
  //  - vnormalize(vcross(brf-brr, blr-brr)) ))
  //  #warning "not a flat bottom quadrilateral"
  //#end
  // should do the other four too.
  // Turns out, extra effort below makes a side of two triangles look okay
  // so we needn't fuss.

  #local BLF2 = cornerball(BLF, BLR, TLF, BRF, Smallr);
  #local TLF2 = cornerball(TLF, TRF, BLF, TLR, Smallr);
  #local TRF2 = cornerball(TRF, BRF, TLF, TRR, Smallr);
  #local BRF2 = cornerball(BRF, BLF, TRF, BRR, Smallr);

  #local BLR2 = cornerball(BLR, TLR, BLF, BRR, Smallr);
  #local TLR2 = cornerball(TLR, BLR, TRR, TLF, Smallr);
  #local TRR2 = cornerball(TRR, BRR, TRF, TLR, Smallr);
  #local BRR2 = cornerball(BRR, TRR, BLR, BRF, Smallr);

  // Draw a two triangles between four points.
  // The two triangles will share a edge between the second and fourth points.
  #macro hexface (
    lf, lr, rr, rf    // rectangle
    )
    // use smallr and Use_saddle from parent scope, ewww
    #local nml_lf = Smallr * vnormalize(vcross(lr-lf, rf-lf));
    #local nml_rr = Smallr * vnormalize(vcross(rf-rr, lr-rr));
    #if (Use_saddle)
      #local nml_lr = Smallr * vnormalize(vcross(rr-lr, lf-lr));
      #local nml_rf = Smallr * vnormalize(vcross(lf-rf, rr-rf));
      saddleup (rf+nml_rf, lf+nml_lf, lr+nml_lr, rr+nml_rr)
    #else
      triangle {rf+nml_lf, lf+nml_lf, lr+nml_lf}
      triangle {lr+nml_rr, rr+nml_rr, rf+nml_rr}
    #end
  #end

  union {
    // faces
    #if (1)
    hexface(TLF2, TLR2, TRR2, TRF2) // top
    hexface(BRF2, BRR2, BLR2, BLF2) // bottom, clockwise from apparent top right
    hexface(TLF2, TRF2, BRF2, BLF2) // front
    hexface(TRR2, TLR2, BLR2, BRR2) // rear, clockwise from apparent top left
    hexface(BRF2, TRF2, TRR2, BRR2) // right
    hexface(BLR2, TLR2, TLF2, BLF2) // left
    #end
    
    // Diagonals between opposite corners on a face.
    // Unnecessary if faces are flat or concave.  Looks damned good if convex.
    #if((Use_saddle = 0) * Smallr )
      cylinder {BLF2, TRF2, Smallr}  cylinder {TLR2, BLF2, Smallr} // front, left
      cylinder {TLR2, BRR2, Smallr}  cylinder {TRF2, BRR2, Smallr} // rear, right
      cylinder {TLR2, TRF2, Smallr}  cylinder {BLF2, BRR2, Smallr} // top, bottom
    #end

    #if (Smallr)
      // corners
      sphere {TLF2, Smallr}  sphere {TLR2, Smallr}  sphere {TRR2, Smallr}  sphere {TRF2, Smallr}
      sphere {BLF2, Smallr}  sphere {BLR2, Smallr}  sphere {BRR2, Smallr}  sphere {BRF2, Smallr}
      // edges
      #if (1)
      cylinder {TLF2, TLR2, Smallr}  cylinder {TLR2, TRR2, Smallr} // around the top
      cylinder {TRR2, TRF2, Smallr}  cylinder {TRF2, TLF2, Smallr}
      cylinder {BLF2, BLR2, Smallr}  cylinder {BLR2, BRR2, Smallr} // around the bot
      cylinder {BRR2, BRF2, Smallr}  cylinder {BRF2, BLF2, Smallr}
      cylinder {TLF2, BLF2, Smallr}  cylinder {TLR2, BLR2, Smallr} // left verticals
      cylinder {TRF2, BRF2, Smallr}  cylinder {TRR2, BRR2, Smallr} // right verticals
      #end
    #end
  }
    
#end

#macro _hexdron2(
  TLF, TLR, TRR, TRF, // top quadrilateral.  Top left front, top left rear, etc.
  BLF, BLR, BRR, BRF, // bottom quadrilateral
  R	          // radius of rounded edges
  )
  
  //#if (vlength(vnormalize(vcross(tlr-tlf, trf-tlf))
  //  - vnormalize(vcross(trf-trr, tlr-trr)) ))
  //  #warning "not a flat top quadrilateral"
  //#end
  //#if (vlength(vnormalize(vcross(blr-blf, brf-blf))
  //  - vnormalize(vcross(brf-brr, blr-brr)) ))
  //  #warning "not a flat bottom quadrilateral"
  //#end
  // should do the other four too.

  // go clockwise, viewed from outside the object
  #local (Ptop, Dtop) = PlaneFrom3Points(TLF, TLR, TRF);
  #local (Pbot, Dbot) = PlaneFrom3Points(BLF, BRF, BLR);
  #local (Pfro, Dfro) = PlaneFrom3Points(TLF, TRF, BLF);
  #local (Plef, Dlef) = PlaneFrom3Points(TLF, BLF, TLR);
  #local (Prea, Drea) = PlaneFrom3Points(TRR, TLR, BRR);
  #local (Prig, Drig) = PlaneFrom3Points(BRF, TRF, TRR);

  #ifdef (CHEAP)
    intersection {
      plane {Ptop, Dtop}
      plane {Pbot, Dbot}
      plane {Pfro, Dfro}
      plane {Plef, Dlef}
      plane {Prea, Drea}
      plane {Prig, Drig}
    }
  #else
    // Find the intersection of the three planes
    #local Det=Matrix3determinatedV(Ptop, Plef, Pfro);
    #if (Det = 0)
      # error concat("pffffpfpfpf Det = 0 C:",
        str(C.x,1,2), ",", str(C.y,1,2), ",",str(C.z,1,2),
        " V1: ", str(V1.x,1,2), ",", str(V1.y,1,2), ",",str(V1.z,1,2),
        " V2: ", str(V2.x,1,2), ",", str(V2.y,1,2), ",",str(V2.z,1,2))
    #end
    #local Vtlf = CornerballP(Ptop, Dtop-R, Plef, Dlef-R, Pfro, Dfro-R);
    #local Vtrf = CornerballP(Ptop, Dtop-R, Prig, Drig-R, Pfro, Dfro-R);
    #local Vtlr = CornerballP(Ptop, Dtop-R, Plef, Dlef-R, Prea, Drea-R);
    #local Vtrr = CornerballP(Ptop, Dtop-R, Prig, Drig-R, Prea, Drea-R);
    #local Vblf = CornerballP(Pbot, Dbot-R, Plef, Dlef-R, Pfro, Dfro-R);
    #local Vbrf = CornerballP(Pbot, Dbot-R, Prig, Drig-R, Pfro, Dfro-R);
    #local Vblr = CornerballP(Pbot, Dbot-R, Plef, Dlef-R, Prea, Drea-R);
    #local Vbrr = CornerballP(Pbot, Dbot-R, Prig, Drig-R, Prea, Drea-R);
    merge {
      intersection { // the interior
        plane {Ptop, Dtop-R}
        plane {Pbot, Dbot-R}
        plane {Pfro, Dfro-R}
        plane {Plef, Dlef-R}
        plane {Prea, Drea-R}
        plane {Prig, Drig-R}
      }
      // normals are pointing out.  Reducing D moves out of object.
      // left wall
      #local (Plt, Dlt) = PlaneFrom3Points(Vtlf, Vtlf+Plef, Vtlr);
      #local (Plb, Dlb) = PlaneFrom3Points(Vblf, Vblr, Vblf+Plef);
      #local (Plf, Dlf) = PlaneFrom3Points(Vtlf, Vblf, Vtlf+Plef);
      #local (Plr, Dlr) = PlaneFrom3Points(Vtlr, Vtlr+Plef, Vblr);
      intersection {
        plane {Plef, Dlef}
        plane {-Plef, -Dlef+2*R}
        plane {Plt, Dlt}
        plane {Plb, Dlb}
        plane {Plr, Dlr}
        plane {Plf, Dlf}
      }
      // right
      #local (Prt, Drt) = PlaneFrom3Points(Vtrf, Vtrr, Vtrf+Prig);
      #local (Prb, Drb) = PlaneFrom3Points(Vbrf, Vbrr, Vbrf-Prig);
      #local (Prf, Drf) = PlaneFrom3Points(Vbrf, Vtrf, Vbrf+Prig);
      #local (Prr, Drr) = PlaneFrom3Points(Vtrr, Vbrr, Vtrr+Prig);
      intersection {
        plane {Prig, Drig} plane {-Prig, -Drig+2*R}
        plane {Prt, Drt} plane {Prb, Drb} plane {Prf, Drf} plane {Prr, Drr}
      }
      // top
      #local (Ptl, Dtl) = PlaneFrom3Points(Vtlf, Vtlr, Vtlf+Ptop);
      #local (Ptr, Dtr) = PlaneFrom3Points(Vtrr, Vtrf, Vtrf+Ptop);
      #local (Ptf, Dtf) = PlaneFrom3Points(Vtrf, Vtlf, Vtrf+Ptop);
      #local (Ptb, Dtb) = PlaneFrom3Points(Vtrr, Vtlr, Vtrr-Ptop); // b==back
      intersection {
        plane {Ptop, Dtop} plane {-Ptop, -Dtop+2*R}
        plane {Ptl, Dtl} plane {Ptr, Dtr} plane {Ptf, Dtf} plane {Ptb, Dtb}
      }
      // bottom
      #local (Pbl, Dbl) = PlaneFrom3Points(Vblr, Vblf, Vblr+Pbot);
      #local (Pbr, Dbr) = PlaneFrom3Points(Vbrf, Vbrr, Vbrf+Pbot);
      #local (Pbf, Dbf) = PlaneFrom3Points(Vbrf, Vblf, Vbrf-Pbot);
      #local (Pbb, Dbb) = PlaneFrom3Points(Vbrr, Vblr, Vbrr+Pbot); // bb==bottom back
      intersection {
        plane {Pbot, Dbot} plane {-Pbot, -Dbot+2*R}
        plane {Pbl, Dbl} plane {Pbr, Dbr} plane {Pbf, Dbf} plane {Pbb, Dbb}
      }
      // front
      #local (Pfl, Dfl) = PlaneFrom3Points(Vblf, Vtlf, Vblf+Pfro);
      #local (Pfr, Dfr) = PlaneFrom3Points(Vbrf, Vtrf, Vbrf-Pfro);
      #local (Pft, Dft) = PlaneFrom3Points(Vtrf, Vtlf, Vtrf-Pfro);
      #local (Pfb, Dfb) = PlaneFrom3Points(Vbrf, Vblr, Vbrf+Pfro);
      intersection {
        plane {Pfro, Dfro} plane {-Pfro, -Dfro+2*R}
        plane {Pfl, Dfl} plane {Pfr, Dfr} plane {Pft, Dft} plane {Pfb, Dfb}
      }
      // rear
      #local (Prl, Drl) = PlaneFrom3Points(Vblr, Vtlr, Vblr-Prea);
      #local (Prr, Drr) = PlaneFrom3Points(Vbrr, Vtrr, Vbrr+Prea);
      #local (Prt, Drt) = PlaneFrom3Points(Vtrr, Vtlr, Vtrr+Prea);
      #local (Prb, Drb) = PlaneFrom3Points(Vbrr, Vblr, Vbrr-Prea); // rb==rear bottom
      intersection {
        plane {Prea, Drea} plane {-Prea, -Drea+2*R}
        plane {Prl, Drl} plane {Prr, Drr} plane {Prt, Drt} plane {Prb, Drb}
      }
      sphere {Vtlf, R}
      sphere {Vtlr, R}
      sphere {Vtrr, R}
      sphere {Vtrf, R}
      sphere {Vblf, R}
      sphere {Vblr, R}
      sphere {Vbrr, R}
      sphere {Vbrf, R}
      cylinder {Vtlf, Vtlr, R} // around the top
      cylinder {Vtlr, Vtrr, R}
      cylinder {Vtrr, Vtrf, R}
      cylinder {Vtrf, Vtlf, R}
      cylinder {Vblf, Vblr, R} // around the bot
      cylinder {Vblr, Vbrr, R}
      cylinder {Vbrr, Vbrf, R}
      cylinder {Vbrf, Vblf, R}
      cylinder {Vtlf, Vblf, R} // four from top to bot
      cylinder {Vtlr, Vblr, R}
      cylinder {Vtrr, Vbrr, R}
      cylinder {Vtrf, Vbrf, R}
    }
  #end

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

  #ifdef (CHEAP)

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
      _hexdron(
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

//======================================================================
//======================================================================
// cornerball, in all its glory.
// Pass in the vertex shared by three trianges (the point of a
// triangular pyramid),then the three other vertices of the pyramid
// in clockwise order when looking from outside the object,
// and cornerball() will return the closest location to the first vertex
// that a ball of the given radius will fit.
//
// Requires normalised plane vectors.
//
#macro cornerball (C, V1, V2, V3, R)

  #local (P1, D1) = PlaneFrom3Points(C, V1, V2);
  #local (P2, D2) = PlaneFrom3Points(C, V2, V3);
  #local (P3, D3) = PlaneFrom3Points(C, V3, V1);

  // Move the planes toward the inside of the shape by the radius of the edging.
  #local D1 = D1 - R;   #local D2 = D2 - R;   #local D3 = D3 - R;
  
  // Find the intersection of the three planes
  //#local Det=Matrix3determinated(P1.x,P1.y,P1.z, P2.x,P2.y,P2.z, P3.x,P3.y,P3.z);
  #local Det=Matrix3determinatedV(P1, P2, P3);
  #if (Det = 0)
  # error concat("pffffpfpfpf Det = 0 C:",
    str(C.x,1,2), ",", str(C.y,1,2), ",",str(C.z,1,2),
    " V1: ", str(V1.x,1,2), ",", str(V1.y,1,2), ",",str(V1.z,1,2),
    " V2: ", str(V2.x,1,2), ",", str(V2.y,1,2), ",",str(V2.z,1,2))
  #end
  // POV planes are P-D=0.  The intersection calculation expects P+D=0.
  #local X = (Matrix3determinated(-D1,P1.y,P1.z, -D2,P2.y,P2.z, -D3,P3.y,P3.z) / -Det);
  #local Y = (Matrix3determinated(P1.x,-D1,P1.z, P2.x,-D2,P2.z, P3.x,-D3,P3.z) / -Det);
  #local Z = (Matrix3determinated(P1.x,P1.y,-D1, P2.x,P2.y,-D2, P3.x,P3.y,-D3) / -Det);

  #ifdef (DDEBUG)
    #warning concat("cornerball C:",
      str(C.x,1,1), ",", str(C.y,1,1), ",",str(C.z,1,1),
      " V1: ", str(V1.x,1,1), ",", str(V1.y,1,1), ",",str(V1.z,1,1),
      " V2: ", str(V2.x,1,1), ",", str(V2.y,1,1), ",",str(V2.z,1,1),
      " V3: ", str(V3.x,1,1), ",", str(V3.y,1,1), ",",str(V3.z,1,1)
      )
  
    intersection {plane {P1, D1} plane {P2, D2} plane {P3, D3}}
  
    #warning concat("P1:", str(P1.x,1,2), ",", str(P1.y,1,2), ",",str(P1.z,1,2), ",", str(D1, 1, 2))
    #warning concat("P2:", str(P2.x,1,2), ",", str(P2.y,1,2), ",",str(P2.z,1,2), ",", str(D2, 1, 2))
    #warning concat("P3:", str(P3.x,1,2), ",", str(P3.y,1,2), ",",str(P3.z,1,2), ",", str(D3, 1, 2))
  
    #warning concat("Det = ", str(Det, 1, 2), " C:",
      str(C.x,1,2), ",", str(C.y,1,2), ",",str(C.z,1,2),
      " V1: ", str(V1.x,1,2), ",", str(V1.y,1,2), ",",str(V1.z,1,2),
      " V2: ", str(V2.x,1,2), ",", str(V2.y,1,2), ",",str(V2.z,1,2),
      " V3: ", str(V3.x,1,2), ",", str(V3.y,1,2), ",",str(V3.z,1,2))
  
    #warning concat("XYZ ", str(X, 1, 4), " ", str(Y, 1, 4), " ", str(Z, 1, 4))
   
    disc {(V1+V2)/2 - P1/10000, P1, 0.1 pigment {rgb <1,0,0>}}
    cylinder {(V1+V2)/2, (V1+V2)/2 + vnormalize(P1)/2, 0.01 pigment {rgb <1,0,0>}}
    disc {(V2+V3)/2 - P2/10000, P2, 0.1 pigment {rgb <0,1,0>}}
    cylinder {(V2+V3)/2, (V2+V3)/2 + vnormalize(P2)/2, 0.01 pigment {rgb <0,1,0>}}
    disc {(V3+V1)/2 - P3/10000, P3, 0.1 pigment {rgb <0,0,1>}}
    cylinder {(V3+V1)/2, (V3+V1)/2 + vnormalize(P3)/2, 0.01 pigment {rgb <0,0,1>}}
    triangle {V1, C, V2}
    triangle {V1, C, V3}
    triangle {V2, C, V3}
  
    sphere { <X, Y, Z>, R*1.001 pigment {rgb <0,0,0>}}
    
    // sphere {C+B, R/2 pigment {rgb <0,1,0>}}
    #warning concat("XYZ ", str(X, 1, 4), " ", str(Y, 1, 4), " ", str(Z, 1, 4))
  #end
  <X, Y, Z>
  
#end

//----------------------------------------------------------------------
// Like cornerball, but using three planes instead of vectors.
// Requires normalised plane vectors.
#macro CornerballP (P1, D1,  P2, D2,  P3, D3)
  #local Det=Matrix3determinatedV(P1, P2, P3);
  #if (Det = 0)
  # error concat("CornerballP Det = 0 ",
    str(P1.x,1,2), ",", str(P1.y,1,2), ",",str(P1.z,1,2),
    " V1: ", str(P2.x,1,2), ",", str(P2.y,1,2), ",",str(P2.z,1,2),
    " V2: ", str(P3.x,1,2), ",", str(P3.y,1,2), ",",str(P3.z,1,2))
  #end
  <(Matrix3determinated(-D1,P1.y,P1.z, -D2,P2.y,P2.z, -D3,P3.y,P3.z) / -Det),
   (Matrix3determinated(P1.x,-D1,P1.z, P2.x,-D2,P2.z, P3.x,-D3,P3.z) / -Det),
   (Matrix3determinated(P1.x,P1.y,-D1, P2.x,P2.y,-D2, P3.x,P3.y,-D3) / -Det)>
  
#end  

