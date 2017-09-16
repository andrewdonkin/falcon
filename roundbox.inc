// Originally:
// POV-Ray 3.6/3.7 include file "Round_Box_Y.inc"                                                                                                                                                // PoVRay 3.6 include File "wheel_000.inc"
// author: Friedrich A, Lohmueller, July-2016
// homepage: http://www.f-lohmueller.de/

// I hacked this about to suit my particular needs.  It is not improved.

//---------------------------------------------------------------------------------------
//#ifndef(  Shapes_Inc_Temp)
//#include "shapes.inc"                                             
//#end 
//#ifndef(  Shapes3_Inc_Temp)
//#include "shapes3.inc"                                             
//#end
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#macro skirted_box_y (  R_Point_1_In, R_Point_2_In,         // box outline
                      Border_Radius_Big,    // radius around vertical edges 
                      Border_Radius_Small,  // radius around horizontal edges
                      Y_Filled___On, // 
                      Merge___On
                   ) //------------------------
//---------------------------------------------
#local D = 0.0000001;  // a very little bit to avoid coincident surfaces!!!

//---------------------------------------------

#if ( abs(Border_Radius_Big) >= abs(Border_Radius_Small))
#local Rmaj = abs(Border_Radius_Big); 
#local Rmin = abs(Border_Radius_Small); 
#end 

#if ( abs(Border_Radius_Big) < abs(Border_Radius_Small)) 
 #warning "\nRound_Box_Y()macro  \n called Border_Radius_Big < Border_Radius_Small,\nradi interchanged\n"
#local Rmaj = abs(Border_Radius_Small); 
#local Rmin = abs(Border_Radius_Big); 
#end 

#if ((Y_Filled___On = 0) & ( Rmaj < 2*Rmin)) // for non filled version
 #local  Rmin = Rmaj/2 - D; 
#end  

#if (Rmin   = 0)
 #local Rmin = 2*D;
#end

// --------------------------------------------------------------
#if ( 2*Rmaj >  (R_Point_2_In.x-R_Point_1_In.x) ) 
#warning "\nRound_Box_Y() macro called with bad x-size:  width in x < 2*Radius_big. Results may not be as expected\n"
#end 

#if ( (2*Rmin) >  (R_Point_2_In.y-R_Point_1_In.y) )     
#warning "\nRound_Box_Y() macro called with bad y-size:  width in y < 2*Radius_big. Results may not be as expected\n"
#end 

#if ( 2*Rmaj >  (R_Point_2_In.z-R_Point_1_In.z) ) 
#warning "\nRound_Box_Y() macro called with bad z-size:  width in z < 2*Radius_small. Results may not be as expected\n"
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
#if (Rmin > D)
#local Quarter_1 = 
//#macro Segment_of_Torus( R_major, R_minor, Segment_Angle)
intersection {
  torus { Rmaj-Rmin, Rmin }
  box { <-Rmaj-Rmin-D, -Rmin-D, -Rmaj-Rmin-D>, <0, Rmin+D, 0> }
  translate<Rmaj,0,Rmaj>
}
#end

//---------------------------------------------------------
#if (Merge___On = 1)  // 1
 merge{   // outer union/merge
#else 
 union{ 
#end 

#if (Y_Filled___On = 0)
difference {
 union {
#end

cylinder{ <P1.x+Rmaj-D, P1.y       , P1.z+Rmaj>,
          <P1.x+Rmaj-D, P2.y-Rmin+D, P1.z+Rmaj>, Rmaj} 
cylinder{ <P1.x+Rmaj-D, P1.y       , P2.z-Rmaj>,
          <P1.x+Rmaj-D, P2.y-Rmin+D, P2.z-Rmaj>, Rmaj} 
 
cylinder{ <P2.x-Rmaj, P1.y       , P1.z+Rmaj>,
          <P2.x-Rmaj, P2.y-Rmin+D, P1.z+Rmaj>, Rmaj} 
cylinder{ <P2.x-Rmaj, P1.y       , P2.z-Rmaj>,
          <P2.x-Rmaj, P2.y-Rmin+D, P2.z-Rmaj>, Rmaj} 

 box{ P1+< Rmaj, 0 , 0 >, P2 - <Rmaj, Rmin, 0 >} //  z
 box{ P1+<0, 0, Rmaj>, P2 - <0, Rmin, Rmaj>} //  x

#if (Y_Filled___On = 1)

 box{ P1+< Rmaj-D, 0    , Rmin  >,P2+<-Rmaj+D,-D    ,-Rmin  >} //  z
 box{ P1+< Rmin, 0    , Rmaj  >, P2 -<Rmin, 0 , Rmaj  >} //  x

 cylinder { <P1.x+Rmaj, P1.y, P1.z+Rmaj>, <P1.x + Rmaj, P2.y, P1.z + Rmaj>, Rmaj-Rmin} // FL
 cylinder { <P2.x-Rmaj, P1.y, P1.z+Rmaj>, <P2.x - Rmaj, P2.y, P1.z + Rmaj>, Rmaj-Rmin} // FR
 cylinder { <P1.x+Rmaj, P1.y, P2.z-Rmaj>, <P1.x + Rmaj, P2.y, P2.z - Rmaj>, Rmaj-Rmin} // FL
 cylinder { <P2.x-Rmaj, P1.y, P2.z-Rmaj>, <P2.x - Rmaj, P2.y, P2.z - Rmaj>, Rmaj-Rmin} // FR

#else // hollow

 } // employment contracts (the end of the union)

 // subtract four small cylinders
 cylinder{ <P1.x+Rmaj, P1.y-D, P1.z+Rmaj>,
           <P1.x+Rmaj, P2.y+D, P1.z+Rmaj>, Rmaj-2*Rmin} 
 cylinder{ <P1.x+Rmaj, P1.y-D, P2.z-Rmaj>,
           <P1.x+Rmaj, P2.y+D, P2.z-Rmaj>, Rmaj-2*Rmin} 
   
 cylinder{ <P2.x-Rmaj, P1.y-D, P1.z+Rmaj>,
           <P2.x-Rmaj, P2.y+D, P1.z+Rmaj>, Rmaj-2*Rmin} 
 cylinder{ <P2.x-Rmaj, P1.y-D, P2.z-Rmaj>,
           <P2.x-Rmaj, P2.y+D, P2.z-Rmaj>, Rmaj-2*Rmin} 

 // subtract the bulk of the interior
 box{ P1+< Rmin*2  , -D    , Rmaj>, P2+<-Rmin*2  , +D    ,-Rmaj> } //  x
 box{ P1+< Rmaj, -D, Rmin*2  >,P2+<-Rmaj+D,-D    ,-Rmin*2  >} //  z

} // end our differences
#end //hollow
 
union{ // horizontal small border cylinders
cylinder{ <P1.x+Rmin,P2.y-Rmin,P1.z+Rmaj-D>,<P1.x+Rmin,P2.y-Rmin,P2.z-Rmaj+D>, Rmin} 
cylinder{ <P2.x-Rmin,P2.y-Rmin,P1.z+Rmaj-D>,<P2.x-Rmin,P2.y-Rmin,P2.z-Rmaj+D>, Rmin} 

cylinder{ <P1.x+Rmaj-D,P2.y-Rmin,P1.z+Rmin>,<P2.x-Rmaj+D,P2.y-Rmin,P1.z+Rmin>, Rmin} 
cylinder{ <P1.x+Rmaj-D,P2.y-Rmin,P2.z-Rmin>,<P2.x-Rmaj+D,P2.y-Rmin,P2.z-Rmin>, Rmin} 
 // pigment {color rgb<1,0.7,0>} 

}

// edges curved
object{ Quarter_1  rotate<0, 0*90,0> translate<P1.x,P2.y-Rmin,P1.z> }
object{ Quarter_1  rotate<0,-1*90,0> translate<P2.x,P2.y-Rmin,P1.z> }
object{ Quarter_1  rotate<0,-2*90,0> translate<P2.x,P2.y-Rmin,P2.z> }
object{ Quarter_1  rotate<0,-3*90,0> translate<P1.x,P2.y-Rmin,P2.z> }

}// end of outer union/merge
#end// of macro ------------------------------------------------------// end of macro

