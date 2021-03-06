// PoVRay 3.7 Scene File "Round_Box_Y_demo01.pov"
// author: Friedrich A, Lohmueller, July-2016
// homepage: http://www.f-lohmueller.de/
//------------------------------------------------------------------------

//hacked to demo my derivative round box with skirtings

#version 3.7;
global_settings { assumed_gamma 1.0 }
#default{ finish{ emission 0.1 diffuse 0.9 }} 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "metals.inc"
#include "golds.inc"
#include "stones.inc"
#include "woods.inc"
#include "shapes.inc"
#include "shapes2.inc"
#include "functions.inc"
#include "math.inc"
#include "transforms.inc"
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#declare Camera_Number = 1 ;
//---------------------------------------------------------------------------------
// camera -------------------------------------------------------------------------
#switch ( Camera_Number )
#case (0)
  #declare Camera_Location = < 0.00, 1.00,-40.00> ;  // front view
  #declare Camera_Look_At  = < 0.00, 1.00,  0.00> ;
  #declare Camera_Angle    =  15 ;
#break
#case (1)
  #declare Camera_Location =  <2.0  , 1.75 ,-2.0> ;  // diagonal view
  //#declare Camera_Location =  <2.0  , 2.75 ,4.0> ;  // diagonal view
  #declare Camera_Look_At  =  < 0.5 , 0.15 , 0.5> ;
  #declare Camera_Angle    =  48 ;
#break
#case (2)
  #declare Camera_Location = < 3.0, 1.0 , 0.0> ;  // right side view
  #declare Camera_Look_At  = < 0.0, 1.0,  0.0> ;
  #declare Camera_Angle    =  90 ;
#break
#case (3)
  #declare Camera_Location = < 0.00, 5.00,  0+0.000> ;  // top view
  #declare Camera_Look_At  = < 0.00, 0.00,  0+0.001> ;
  #declare Camera_Angle    = 90 ;
#break
#else
  #declare Camera_Location = < 0.00, 1.00, -3.50> ;  // front view
  #declare Camera_Look_At  = < 0.00, 1.00,  0.00> ;
  #declare Camera_Angle    =  75 ;
#break
#break
#end // of "#switch ( Camera_Number )"  
//--------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------<<<<<<<<<<<<<<
//----------------------------------------------------------------------------------------------<<<<<<<<<<<<<<
camera{ // ultra_wide_angle // orthographic 
        location Camera_Location
        right    x*image_width/image_height
        angle    Camera_Angle
        look_at  Camera_Look_At
      }
//------------------------------------------------------------------------
// sun -------------------------------------------------------------------
light_source{<-1000, 2500,-2000> color rgb<1,1,1>*0.9}                // sun 
light_source{ Camera_Location   color rgb<0.9,0.9,1>*0.1 shadowless}// flash
// sky -------------------------------------------------------------------
sky_sphere{ pigment{ gradient <0,1,0>
                     color_map{ [0   color rgb<1,1,1>         ]//White
                                [0.4 color rgb<0.24,0.34,0.66>*0.3]//~Navy
                                [0.6 color rgb<0.24,0.34,0.66>*0.3]//~Navy
                                [1.0 color rgb<1,1,1>         ]//White
                              }
                     scale 2 
                     rotate<-30,0,0>}
           } // end of sky_sphere 
//------------------------------------------------------------------------

//------------------------------ the Axes --------------------------------
//------------------------------------------------------------------------
#macro Axis_( AxisLen, Dark_Texture,Light_Texture) 
 union{
    cylinder { <0,-AxisLen,0>,<0,AxisLen,0>,0.05
               texture{checker texture{Dark_Texture } 
                               texture{Light_Texture}
               scale 0.5
               translate<0.1,0,0.1>}
             }
    cone{<0,AxisLen,0>,0.2,<0,AxisLen+0.7,0>,0
          texture{Dark_Texture}
         }
     } // end of union                   
#end // of macro "Axis()"
//------------------------------------------------------------------------
#macro AxisXYZ( AxisLenX, AxisLenY, AxisLenZ, Tex_Dark, Tex_Light)
//--------------------- drawing of 3 Axes --------------------------------
union{
#if (AxisLenX != 0)
 object { Axis_(AxisLenX, Tex_Dark, Tex_Light)   rotate< 0,0,-90>}// x-Axis
 text   { ttf "arial.ttf",  "x",  0.15,  0  texture{Tex_Dark} 
          rotate<20,-35,0> scale 0.65 translate <AxisLenX+0.05,0.4,-0.10> no_shadow}
#end // of #if 
#if (AxisLenY != 0)
 object { Axis_(AxisLenY, Tex_Dark, Tex_Light)   rotate< 0,0,  0>}// y-Axis
 text   { ttf "arial.ttf",  "y",  0.15,  0  texture{Tex_Dark}    
          rotate<10,0,0> scale 0.65 translate <-0.65,AxisLenY+0.30,-0.10>  rotate<0,-35,0> no_shadow}
#end // of #if 
#if (AxisLenZ != 0)
 object { Axis_(AxisLenZ, Tex_Dark, Tex_Light)   rotate<90,0,  0>}// z-Axis
 text   { ttf "arial.ttf",  "z",  0.15,  0  texture{Tex_Dark}
          rotate<20,-35,0> scale 0.85 translate <-0.75,0.2,AxisLenZ+0.10> no_shadow}
#end // of #if 
} // end of union
#end// of macro "AxisXYZ( ... )"
//------------------------------------------------------------------------

#declare Texture_A_Dark  = texture {
                               pigment{ color rgb<1,0.4,0>}
                               finish { phong 1}
                             }
#declare Texture_A_Light = texture { 
                               pigment{ color rgb<1,1,1>}
                               finish { phong 1}
                             }



object{ AxisXYZ( 5.0, 3.0, 13 Texture_A_Dark, Texture_A_Light) scale 0.25}
//-------------------------------------------------- end of coordinate axes


// ground -----------------------------------------------------------------
//---------------------------------<<< settings of squared plane dimensions
#declare RasterScale = 0.1 ;
#declare RasterHalfLine  = 0.065 ;  
#declare RasterHalfLineZ = 0.065 ; 
//-------------------------------------------------------------------------
#macro Raster(RScale, HLine) 
       pigment{ gradient x scale RScale
                color_map{[0.000   color rgbt<1,1,1,0>*0.6]
                          [0+HLine color rgbt<1,1,1,0>*0.6]
                          [0+HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt<1,1,1,0>*0.6]
                          [1.000   color rgbt<1,1,1,0>*0.6]} }
 #end// of Raster(RScale, HLine)-macro    
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
plane { <0,1,0>, 0    // plane with layered textures
        texture { pigment{color White*1.1}
                  finish {emission 0.45 diffuse 0.85}}
        texture { Raster(RasterScale,RasterHalfLine ) rotate<0,0,0> }
        texture { Raster(RasterScale,RasterHalfLineZ) rotate<0,90,0>}

        texture { Raster(10*RasterScale,0.2*RasterHalfLine ) rotate<0,0,0> }
        texture { Raster(10*RasterScale,0.2*RasterHalfLineZ) rotate<0,90,0>}

        rotate<0,0,0>
      }
   */
//------------------------------------------------ end of squared plane XZ

//--------------------------------------------------------------------------
//---------------------------- objects in scene ----------------------------
//--------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
#include "skirtbox.inc"
//---------------------------------------------------------------------------------------
object{ Skirted_box_y ( <-0.1,0.1,0.0>, <-0.9 ,0.5, 1.5>,  // Point_1, Point_2 of outline box
                      0.180, // Border_Radius_Big,  // big radius of vertical borders
                      0.040, // Border_Radius_Small,// small rad�us of horizontal borders
                      0.02, //skirt radius
                      0 // Y_Filled___On
                    ) //----------------------------------------
        texture {
              pigment{ color rgb <1,1,1 >*0.85 } 
              finish { phong 1 reflection 0.1 } 
        } // end of texture      
       scale<1,1,1>  rotate< 0,0,0> translate<0.0,0.0,0.0>
  }//------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

object{ Skirted_lozenge_y (
                      1.5, 0.4, // length (Z), top
                      0.18, 0.04, // big end radius, smaller top radius
                      0.02, // skirting
                      0 // Y_Filled___On,
                    ) //----------------------------------------
        texture {
              pigment{ color rgb <1,1,1 >*0.85 } 
              finish { phong 1 reflection 0.1 } 
        } // end of texture      
       scale<1,1,1>  rotate< 0,0,0> translate <0.5,0.1,0.0>
  }//------------------------------------------------------------------------------------

object {Skirted_blister (1, 0.4, 0.2)
        texture {
              pigment{ color rgb <1,1,1 >*0.85 } 
              finish { phong 1 reflection 0.1 } 
        } // end of texture      
  translate <1.3,0.1,0>
}
plane {y, 0.1
        texture {
              pigment{ color rgb <1,1,1 >*0.85 } 
              finish { phong 1 reflection 0.1 } 
        } // end of texture      
}  
