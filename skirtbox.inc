// Originally:
// POV-Ray 3.6/3.7 include file "Round_Box_Y.inc"                                                                                                                                                // PoVRay 3.6 include File "wheel_000.inc"
// author: Friedrich A, Lohmueller, July-2016
// homepage: http://www.f-lohmueller.de/

// I hacked this about to suit my particular needs.  It is not improved.
// The radius is removed from the bottom of the object:  it sits on a plane.
// The macros are redone in a way that more sense to me, and with fewer objects.
// I have not tested with transparent objects so there could be coincident surfaces.
// The parameter checking is all Friedrich's.

// This file needs a lot of work.  Capitalise, rename vars, indent.

#ifndef (Skirtbox_inc)
#declare Skirtbox_inc = version;

// ############################################################################
// ############################################################################
// ############################################################################
// One way of merging a block into the plane by swelling it as it nears y=0.
// It is not perfectly continuous - see skirted_box for that.
// Centred on the origin, only goes to half of Box.y.
// Containing box unnecessarily big for Rbox < 1.
// Really slow.
// For these reasons, don't use it.
#macro Skirted_box2(Box
                    , Rbox  //
                    , skirting // (0, 1].  1 is biggish, 0.1 good, 0.01 quite tight.
                    )
  #local Boxx=Box.x; // functions don't like vectors.
  #local Boxy=Box.y;
  #local Boxz=Box.z;
  isosurface  {
    function {
        1 + 0.001
        - pow(0.01, f_rounded_box(x,y,z, Rbox, Boxx/2, Boxy/2, Boxz/2))
        - pow(0.01, (y+Boxy/20)*Boxy*3)
    }
    // centred on origin, spanning
    contained_by {box {<-Boxx/1.1, -Boxy/2, -Boxz/1.1>,
                       <Boxx/1.1, Boxy/2, Boxz/1.1>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001*Rbox // default is fine until you stitch them together
  }
#end

// ############################################################################
// ############################################################################
// ############################################################################
// Another way: smooth transient between the plane and the box skirting
// using a the inside of a torus as a skirting.
// bounded_by {box {0, Box}}
#macro Skirted_box(Box                 // 3-vector of box dimensions
                   , Rbox          // radius of box's edges
                   , R)           // radius of skirting
  #local Boxx=Box.x - 2 * R; // functions don't like vectors.
  #local Boxy=Box.y;
  #local Boxz=Box.z - 2 * R;
  union {
    isosurface  {
      function {f_rounded_box(x-Boxx/2, y-Boxy/2, z-Boxz/2, Rbox, Boxx/2, Boxy/2, Boxz/2)
        // + f_noise3d(x*10, y*20, z*20)/40
        }
      contained_by {box {<0, 0, 0>,
                         <Boxx, Boxy, Boxz>}}
      evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
      accuracy 0.000001
      translate <R, 0, R>
    }
    difference {
      merge {
        // these four cylinders lying at the base of the box
        // will get another cylinder subtracted later
        // to form a radiused skirting.
        Xcyl(<R+Rbox, 0, R>, Box.x - 2*(R+Rbox), R) // near
        Zcyl(<R, 0, R+Rbox>, Box.z - 2*(R+Rbox), R) // left 
        Xcyl(<R+Rbox, 0, Box.z-R>, Box.x - 2*(R+Rbox), R) // rear
        Zcyl(<Box.x-R, 0, R+Rbox>, Box.z - 2*(R+Rbox), R) // right
        
        // these four hockey pucks will have quarter of a torus taken out of them
        Ycyl(<R+Rbox,       0, Rbox+R>,       R, R+Rbox) // front left
        Ycyl(<Box.x-Rbox-R, 0, Rbox+R>,       R, R+Rbox) // front right
        Ycyl(<R+Rbox,       0, Box.z-Rbox-R>, R, R+Rbox) // rear left
        Ycyl(<Box.x-Rbox-R, 0, Box.z-Rbox-R>, R, R+Rbox) // rear right
      }
      Xcyl(<0, R, 0>, Box.x, R) // front
      Zcyl(<0, R, 0>, Box.z, R) // left
      Xcyl(<0, R, Box.z>, Box.x, R) // rear
      Zcyl(<Box.x, R, 0>, Box.z, R) // right
      object {Segment_of_Torus(R+Rbox, R, 90) rotate 90*y translate  <R+Rbox, R, R+Rbox> } // front left
      object {Segment_of_Torus(R+Rbox, R, 90)             translate  <Box.x-Rbox-R, R, R+Rbox> } // front right
      object {Segment_of_Torus(R+Rbox, R, 90) rotate 180*y translate <R+Rbox, R, Box.z-R-Rbox> } // rear left
      object {Segment_of_Torus(R+Rbox, R, -90)             translate <Box.x-Rbox-R, R, Box.z-R-Rbox> } // rear right
    }
  }
#end



// ############################################################################
// ############################################################################
// ############################################################################
#macro Skirted_box_y (  R_Point_1_In, R_Point_2_In,         // box outline
                      Border_Radius_Big,    // radius around vertical edges 
                      Border_Radius_Small,  // radius around horizontal edges
                      Rskirt,               // radius of skirt
                      Y_Filled___On // 
                   ) //------------------------
  //---------------------------------------------
  #local D = 0.0000001;  // a very little bit to avoid coincident surfaces!!!

  //---------------------------------------------

  #if ( abs(Border_Radius_Big) >= abs(Border_Radius_Small))
    #local Rmaj = abs(Border_Radius_Big); 
    #local Rmin = abs(Border_Radius_Small); 
  #end 

  #if ( abs(Border_Radius_Big) < abs(Border_Radius_Small)) 
    #warning "\nskirted_box_y() macro  \n called Border_Radius_Big < Border_Radius_Small,\nradi interchanged\n"
    #local Rmaj = abs(Border_Radius_Small); 
    #local Rmin = abs(Border_Radius_Big); 
  #end 

  // If not filled, the thickness of the wall has to be <= radius of the corner.
  #if ((Y_Filled___On = 0) & ( Rmaj < 2*Rmin))
    #warning "\nskirted_box_y() macro  \n called minor radius too large for hollow\nradi reduced\n"
    #local  Rmin = Rmaj/2 - D; 
  #end  

  #if (Rmin   = 0)
    #local Rmin = 2*D;
  #end

  // --------------------------------------------------------------
  #if ( 2*Rmaj >  (R_Point_2_In.x-R_Point_1_In.x) ) 
    #warning "\nskirted_box_y() macro called with bad x-size:  width in x < 2*Radius_big. Results may not be as expected\n"
  #end 

  #if ( Rmin >  (R_Point_2_In.y-R_Point_1_In.y) )     
    #warning "\nskirted_box_y() macro called with bad y-size:  width in y < 2*Radius_big. Results may not be as expected\n"
  #end 

  #if ( 2*Rmaj >  (R_Point_2_In.z-R_Point_1_In.z) ) 
    #warning "\nskirted_box_y() macro called with bad z-size:  width in z < 2*Radius_small. Results may not be as expected\n"
  #end 
  // --------------------------------------------------------------
  // right order of the coordinates:
  #local P1 = < min(R_Point_1_In.x, R_Point_2_In.x),
  min(R_Point_1_In.y, R_Point_2_In.y), 
  min(R_Point_1_In.z, R_Point_2_In.z)>;  
  #local P2 = < max(R_Point_1_In.x, R_Point_2_In.x, min(R_Point_1_In.x, R_Point_2_In.x)+2*Rmaj),
  max(R_Point_1_In.y, R_Point_2_In.y, min(R_Point_1_In.y, R_Point_2_In.y)+2*Rmin), 
  max(R_Point_1_In.z, R_Point_2_In.z, min(R_Point_1_In.z, R_Point_2_In.z)+2*Rmaj)>; 

  //-----------------------------------------------------
  #local Quarter_1 = 
  //#macro Segment_of_Torus( R_major, R_minor, Segment_Angle)
  intersection {
    torus { Rmaj-Rmin, Rmin sturm }
    box { <-Rmaj-Rmin-D, -Rmin-D, -Rmaj-Rmin-D>, <0, Rmin+D, 0> }
    translate<Rmaj,0,Rmaj>
  }

  //---------------------------------------------------------
  // merge/union of 
  // - four large corner cylinders (large means going to the X and Z extremities)
  // - two large boxes
  // if filled in
  // - two smaller boxes going to the extreme Y this time
  // - four smaller corner cylinders going to extreme Y
  // if hollow, subtract
  // - four smaller tall cylinders
  // - two boxes even smaller that those used in the fill-in.
  // whether hollow or filled, add
  // - four cylinders running along the top edges
  // - four quarter-tori at each top corner

  union{ 

    #if (Y_Filled___On = 0)
      difference {
        union {
    #end

          cylinder{ <P1.x+Rmaj-D, P1.y       , P1.z+Rmaj>,
                    <P1.x+Rmaj-D, P2.y-Rmin+D, P1.z+Rmaj>, Rmaj} 
          cylinder{ <P1.x+Rmaj-D, P1.y       , P2.z-Rmaj>,
                    <P1.x+Rmaj-D, P2.y-Rmin+D, P2.z-Rmaj>, Rmaj} 
          
          cylinder{ <P2.x-Rmaj  , P1.y       , P1.z+Rmaj>,
                    <P2.x-Rmaj  , P2.y-Rmin+D, P1.z+Rmaj>, Rmaj} 
          cylinder{ <P2.x-Rmaj  , P1.y       , P2.z-Rmaj>,
                    <P2.x-Rmaj  , P2.y-Rmin+D, P2.z-Rmaj>, Rmaj} 

          box{ P1+< Rmaj, 0 , 0 >, P2 - <Rmaj, Rmin, 0 >} // z: narrow and deep
          box{ P1+<0, 0, Rmaj>,    P2 - <0, Rmin, Rmaj>}  // x: wide and shallow

          #if (Y_Filled___On = 1)

            // full-height boxes.
            box{ P1+< Rmaj-D, 0, Rmin >, P2 + <-Rmaj+D, -D, -Rmin >} // z: narrow and deep
            box{ P1+< Rmin  , 0, Rmaj >, P2 - <Rmin,    0 ,  Rmaj >} //  x: wide and shallow

            cylinder { <P1.x+Rmaj, P1.y, P1.z+Rmaj>, <P1.x + Rmaj, P2.y, P1.z + Rmaj>, Rmaj-Rmin} // FL
            cylinder { <P2.x-Rmaj, P1.y, P1.z+Rmaj>, <P2.x - Rmaj, P2.y, P1.z + Rmaj>, Rmaj-Rmin} // FR
            cylinder { <P1.x+Rmaj, P1.y, P2.z-Rmaj>, <P1.x + Rmaj, P2.y, P2.z - Rmaj>, Rmaj-Rmin} // FL
            cylinder { <P2.x-Rmaj, P1.y, P2.z-Rmaj>, <P2.x - Rmaj, P2.y, P2.z - Rmaj>, Rmaj-Rmin} // FR

          #else // hollow

        } // employment contracts (the end of the union)

            // subtract four small cylinders.
            // the wall thickness is double Rmin
            cylinder{ <P1.x+Rmaj, P1.y-D, P1.z+Rmaj>,
                      <P1.x+Rmaj, P2.y+D, P1.z+Rmaj>, Rmaj-2*Rmin} 
            cylinder{ <P1.x+Rmaj, P1.y-D, P2.z-Rmaj>,
                      <P1.x+Rmaj, P2.y+D, P2.z-Rmaj>, Rmaj-2*Rmin} 
            
            cylinder{ <P2.x-Rmaj, P1.y-D, P1.z+Rmaj>,
                      <P2.x-Rmaj, P2.y+D, P1.z+Rmaj>, Rmaj-2*Rmin} 
            cylinder{ <P2.x-Rmaj, P1.y-D, P2.z-Rmaj>,
                      <P2.x-Rmaj, P2.y+D, P2.z-Rmaj>, Rmaj-2*Rmin} 

            // subtract the bulk of the interior
            box{ P1+< Rmin*2, -D, Rmaj  >, P2+<-Rmin*2, +D, -Rmaj   >} //  x: wide
            box{ P1+< Rmaj  , -D, Rmin*2>, P2+<-Rmaj+D, -D, -Rmin*2 >} //  z: deep

      } // end our differences
          #end //hollow
    
    // horizontal small border cylinders on top
    cylinder{ <P1.x+Rmin,P2.y-Rmin,P1.z+Rmaj-D>,<P1.x+Rmin,P2.y-Rmin,P2.z-Rmaj+D>, Rmin} 
    cylinder{ <P2.x-Rmin,P2.y-Rmin,P1.z+Rmaj-D>,<P2.x-Rmin,P2.y-Rmin,P2.z-Rmaj+D>, Rmin} 

    cylinder{ <P1.x+Rmaj-D,P2.y-Rmin,P1.z+Rmin>,<P2.x-Rmaj+D,P2.y-Rmin,P1.z+Rmin>, Rmin} 
    cylinder{ <P1.x+Rmaj-D,P2.y-Rmin,P2.z-Rmin>,<P2.x-Rmaj+D,P2.y-Rmin,P2.z-Rmin>, Rmin} 
    // pigment {color rgb<1,0.7,0>} 

    // edges curved
    object{ Quarter_1  rotate<0, 0*90,0> translate<P1.x,P2.y-Rmin,P1.z> }
    object{ Quarter_1  rotate<0,-1*90,0> translate<P2.x,P2.y-Rmin,P1.z> }
    object{ Quarter_1  rotate<0,-2*90,0> translate<P2.x,P2.y-Rmin,P2.z> }
    object{ Quarter_1  rotate<0,-3*90,0> translate<P1.x,P2.y-Rmin,P2.z> }


    // Now the skirt

    #local Quarter_2 = 
    intersection {
      torus { Rmaj, Rskirt sturm}
      box { <-Rmaj-Rskirt-D, 0, -Rmaj-Rskirt-D>, <0, Rskirt+Rskirt+D, 0> }
    }
    #local Quarter_3 = 
    intersection {
      torus { Rmaj+Rskirt, Rskirt sturm }
      box { <-Rmaj-Rskirt*2, -Rskirt-D, -Rmaj-Rskirt*2>, <0, Rskirt+D, 0> }
    }

    difference {
      union {
        // these four cylinders lying at the base of the box
        // will get another cylinder subtracted later
        // to form a radiused skirting.
        cylinder {<P1.x+Rmaj, P1.y, P1.z>, <P2.x-Rmaj, P1.y, P1.z>, Rskirt} // near
        cylinder {<P1.x, P1.y, P1.z+Rmaj>, <P1.x, P1.y, P2.z-Rmaj>, Rskirt} // left
        cylinder {<P1.x+Rmaj, P1.y, P2.z>, <P2.x-Rmaj, P1.y, P2.z>, Rskirt} // rear
        cylinder {<P2.x, P1.y, P1.z+Rmaj>, <P2.x, P1.y, P2.z-Rmaj>, Rskirt} // right
        
        // these four quarter-tori will have another quarter torus taken out of them
        object {Quarter_2 rotate   0*y translate <P1.x+Rmaj, P1.y, P1.z+Rmaj> } // near left
        object {Quarter_2 rotate -90*y translate <P2.x-Rmaj, P1.y, P1.z+Rmaj> } // near right
        object {Quarter_2 rotate  90*y translate <P1.x+Rmaj, P1.y, P2.z-Rmaj> } // far left
        object {Quarter_2 rotate 180*y translate <P2.x-Rmaj, P1.y, P2.z-Rmaj> } // far right
      }
      cylinder {<P1.x+Rmaj-D, P1.y+Rskirt, P1.z-Rskirt>,
                <P2.x-Rmaj+D, P1.y+Rskirt, P1.z-Rskirt>, Rskirt} // near
      cylinder {<P1.x-Rskirt, P1.y+Rskirt, P1.z+Rmaj-D>,
                <P1.x-Rskirt, P1.y+Rskirt, P2.z-Rmaj+D>, Rskirt} // left
      cylinder {<P1.x+Rmaj-D, P1.y+Rskirt, P2.z+Rskirt>,
                <P2.x-Rmaj+D, P1.y+Rskirt, P2.z+Rskirt>, Rskirt} // far
      cylinder {<P2.x+Rskirt, P1.y+Rskirt, P1.z+Rmaj-D>,
                <P2.x+Rskirt, P1.y+Rskirt, P2.z-Rmaj+D>, Rskirt} // right
      object {Quarter_3 rotate 0*y translate <P1.x+Rmaj, P1.y+Rskirt, P1.z+Rmaj> } // near left
      object {Quarter_3 rotate -90*y translate <P2.x-Rmaj, P1.y+Rskirt, P1.z+Rmaj> } // near right
      object {Quarter_3 rotate 90*y translate <P1.x+Rmaj, P1.y+Rskirt, P2.z-Rmaj> } // far left
      object {Quarter_3 rotate 180*y translate <P2.x-Rmaj, P1.y+Rskirt, P2.z-Rmaj> } // far right
      plane {y, P1.y}
    }
  } // end of outer union/merge
#end


// This object is centred on x=0
// ############################################################################
// ############################################################################
// ############################################################################
#macro Skirted_lozenge_y (
                      Z, Y,          // length and height
                      End_Radius,    // radius around ends
                      Top_Radius,  // radius around top
                      Rskirt,  // radius of skirt
                      Y_Filled___On // 
                   )

  #local D = 0.0000001;  // a very little bit to avoid coincident surfaces!!!

  #if ( abs(End_Radius) >= abs(Top_Radius))
    # local Rmaj = abs(End_Radius); 
    # local Rmin = abs(Top_Radius); 
  #else
    # warning "\nskirted_lozenge_y() macro \n called End_Radius < Top_Radius,\nradi interchanged\n"
    # local Rmaj = abs(Top_Radius); 
    # local Rmin = abs(End_Radius); 
  #end 

  #if (Rmin   = 0)
    #local Rmin = 2*D;
  #end

  // --------------------------------------------------------------
  #if ( 2*Rmaj > Z ) 
    #error "\nskirted_lozenge_y() macro called with bad z-size:  < 2*End_Radius.\n"
  #end 

  #if ( (Rmin) >  Y )     
    #error "\nskirted_lozenge_y() macro called with bad y-size:  < 2*Top_Radius.\n"
  #end 

  //---------------------------------------------------------
  // not including the skirt, it goes like this:
  // merge/union of 
  // - two large corner cylinders (large means going to the X extremities)
  // - one large (but short) box
  // if filled in
  // - one smaller box going to the full height this time
  // - two smaller corner cylinders going the full height
  // if hollow, subtract
  // - two even smaller tall cylinders
  // - a box even smaller that that used in the fill-in.
  // whether hollow or filled, add
  // - two cylinders running along the top edges, +/- x
  // - two half-tori at each end of the top

  union {

    #if (Y_Filled___On = 0)
      difference {
        union {
    #end

          // First the skirt.
          // It leaves two bits of tori inside that we will scoop out later if hollow
          
          #local Half = intersection {
            torus { Rmaj+Rskirt, Rskirt sturm }
            plane {z, 0}
          }
          
          difference {
            union {
              // these two cylinders lying at the base of the box
              // will get another cylinder subtracted later
              // to form a radiused skirting.
              cylinder {<-Rmaj, 0, Rmaj>, <-Rmaj, 0, Z-Rmaj>, Rskirt} // left
              cylinder {< Rmaj, 0, Rmaj>, < Rmaj, 0, Z-Rmaj>, Rskirt} // right
              
              // these two half-tori will have another taken out of them
              object {torus { Rmaj, Rskirt } translate <0, 0, Rmaj> sturm } // near
              object {torus { Rmaj, Rskirt } translate <0, 0, Z-Rmaj> sturm } // near
            }
            cylinder {<-Rmaj-Rskirt, Rskirt, Rmaj>, <-Rmaj-Rskirt, Rskirt, Z-Rmaj>, Rskirt} // left
            cylinder {< Rmaj+Rskirt, Rskirt, Rmaj>, < Rmaj+Rskirt, Rskirt, Z-Rmaj>, Rskirt} // right
            object {Half rotate 0*y translate <0, Rskirt, Rmaj+D> } // near
            object {Half rotate 180*y translate <0, Rskirt, Z-Rmaj-D> } // near
            plane {y, 0}
          }


          // Two largest cylinders, short
          cylinder{ <0, 0, Rmaj>, <0, Y-Rmin, Rmaj>, Rmaj}
          cylinder{ <0, 0, Z-Rmaj>, <0, Y-Rmin, Z-Rmaj>, Rmaj}

          box{ <-Rmaj, 0 , Rmaj>, <Rmaj, Y-Rmin, Z-Rmaj>} // coincident


          #if (Y_Filled___On = 1)

            box{ <-Rmaj+Rmin, 0, Rmaj>, <Rmaj - Rmin, Y, Z - Rmaj>}

            cylinder{ <0, 0, Rmaj>, <0, Y, Rmaj>, Rmaj-Rmin}
            cylinder{ <0, 0, Z-Rmaj>, <0, Y, Z-Rmaj>, Rmaj-Rmin}

          #else // hollow

        } // the end of the union

            // subtract two small cylinders
            cylinder{ <0, -D, Rmaj>, <0, Y, Rmaj>, Rmaj-Rmin*2}
            cylinder{ <0, -D, Z-Rmaj>, <0, Y, Z-Rmaj>, Rmaj-Rmin*2}

            // subtract the rest of the interior
            box{ <-(Rmaj - (Rmin*2)), -D, Rmaj>, <Rmaj - Rmin*2, Y, Z - Rmaj>}

      } // end our differences
          #end //hollow

    //cap it 
    // horizontal small border cylinders on top
    cylinder{ <-Rmaj + Rmin, Y-Rmin, Rmaj>, <-Rmaj+Rmin, Y-Rmin, Z-Rmaj>, Rmin }
    cylinder{ < Rmaj - Rmin, Y-Rmin, Rmaj>, < Rmaj-Rmin, Y-Rmin, Z-Rmaj>, Rmin }

    // end caps
    #local End1 = torus { Rmaj-Rmin, Rmin sturm };
    #if (Y_Filled___On = 1)
      # local End2 = End1;
    #else // hollow
      # local End1 = intersection { object {End1} plane { z, 0 } }
      # local End2 = object { End1 rotate 180*y };
    #end // hollow

    object{ End1 translate<0,Y-Rmin,Rmaj> }
    object{ End2 translate<0,Y-Rmin,Z-Rmaj> }

  } // end of outer union/merge
#end // of macro

// ############################################################################
// ############################################################################
// ############################################################################
// Two spheres joined by a cylinder, with a skirting.
// Bounded by X -R-Rskirt to R+Rskirt; Y 0 to R;  Z -Rskirt to Z+Rskirt
#macro Skirted_blister (Z,         // overall length not incl skirt
                      R,    // radius around ends
                      Rskirt  // radius of skirt
                   ) //------------------------

  //#local Rd = R * cos(asin(Rskirt/R));
  #local D = 0.0000001;  // a very little bit to avoid coincident surfaces!!!
  #local Theta = asin(Rskirt/(R+Rskirt));
  #local Rdskirt = (R+Rskirt) * cos(Theta);
  #local Rd = Rdskirt-Rskirt;
  #local Yd = Rskirt * (R/(R+Rskirt));

  #local Half = intersection {
    torus { Rdskirt, Rskirt sturm }
    plane {z, 0}
  }

  intersection {
    union {
      sphere { <0, 0, R> R }
      sphere { <0, 0, Z-R> R }
      cylinder { <0, 0, R>, <0, 0, Z-R>, R }
      
      // the skirt
      difference {
        union {
          cylinder {<-Rd, 0, R>, <-Rd, 0, Z-R>, Rskirt}
          cylinder {<Rd, 0, R>, <Rd, 0, Z-R>, Rskirt}
          torus {Rd, Rskirt sturm translate <0, 0, R>}
          torus {Rd, Rskirt sturm translate <0, 0, Z-R>}
        }
        cylinder {<-Rdskirt, Rskirt, R>,   <-Rdskirt, Rskirt, Z-R>, Rskirt}
        cylinder {< Rdskirt, Rskirt, R-D>, < Rdskirt, Rskirt, Z-R+D>, Rskirt}
        object {Half translate <0, Rskirt, R+D>}
        object {Half rotate 180*y translate <0, Rskirt, Z-R-D>}
        plane {-y, -Yd}
      }
    }
    plane {-y, 0}
  }

#end // skirted_blister

#end // skirtbox.inc
