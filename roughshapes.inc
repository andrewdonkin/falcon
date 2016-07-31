// Author:  Andrew Donkin
//
// This file contains macros for creating smoothed and roughed-up geometric shapes.
//

#include "shapes3.inc"
#include "arrays.inc"

#declare rseed = seed(1);
#declare E = 0.0001; //psilon

//=====================================================
#macro RandArray (RA, Scaler)
  #for (I, 0, dimension_size(RA, 1) - 1)
    #local RA[I] = rand(rseed) * Scaler;
  #end // for
  Sort_Array(RA)
#end

//=====================================================
#macro RandLimitedGapArray (RA, Scaler, MaxGapRatio)
  RandArray(RA, Scaler)
  #for (I, 0, dimension_size(RA, 1) - 2, 2)
    #if ((RA[I+1] - RA[I]) > Scaler*MaxGapRatio)
      #local RA[I+1] = RA[I] + rand(rseed) * Scaler*MaxGapRatio;
    #end
  #end // for
#end

//=====================================================
// roughbox
//
//
#macro roughbox (corner2, Rsmall, NumChipsX, NumBulges)

//  #local num_chips_y = abs(corner2.x) / num_chips_x * abs(corner2.y);
//  #local num_chips_z = abs(corner2.x) / num_chips_x * abs(corner2.z);

#local NumChipsX = ceil(NumChipsX);
#local NumBulges = ceil(NumBulges);

#local NumBulgesZ = ceil(abs(NumBulges / corner2.x * corner2.z));

#local XX=corner2.x-Rsmall;
#local YY=corner2.y-Rsmall;
#local ZZ=corner2.z-Rsmall;

#warning concat("Corner2 ", str(corner2.x,0,3), " NumBulges ", str(NumBulges, 0, 0))

#if (NumChipsX > 0) difference #else object #end
{
  union {
    box {<Rsmall, 0, Rsmall>, <XX, corner2.y, ZZ>} //tall
    box {<0, 0, Rsmall>, <corner2.x, YY, ZZ>} // wide
    box {<Rsmall, 0, 0>, <XX, YY, corner2.z>} // deep
    cylinder {0, <0, YY, 0>, Rsmall translate <Rsmall, 0, Rsmall>} // front left vertical
    cylinder {0, <0, YY, 0>, Rsmall translate <Rsmall, 0, corner2.z-Rsmall>} // back left vert
    cylinder {0, <0, YY, 0>, Rsmall translate <corner2.x-Rsmall, 0, Rsmall>} // front right vert
    cylinder {0, <0, YY, 0>, Rsmall translate <corner2.x-Rsmall, 0, corner2.z-Rsmall>} // back right vert
    cylinder {0, (corner2.x-Rsmall*2) * x, Rsmall translate <Rsmall, corner2.y-Rsmall, Rsmall>} //top front horiz
    cylinder {0, (corner2.x-Rsmall*2) * x, Rsmall translate <Rsmall, corner2.y-Rsmall, corner2.z-Rsmall>} //top back horiz
    cylinder {0, (corner2.z-Rsmall*2) * z, Rsmall translate <Rsmall, corner2.y-Rsmall, Rsmall>} //top left horiz
    cylinder {0, (corner2.z-Rsmall*2) * z, Rsmall translate <corner2.x-Rsmall, corner2.y-Rsmall, Rsmall>} //top right horiz
    sphere {<Rsmall, corner2.y-Rsmall, Rsmall>, Rsmall} // left front corner
    sphere {<Rsmall, corner2.y-Rsmall, corner2.z-Rsmall>, Rsmall} // left rear corner
    sphere {<corner2.x-Rsmall, corner2.y-Rsmall, Rsmall>, Rsmall} // right front corner
    sphere {corner2-Rsmall, Rsmall} // right rear corner
    // all that effort to build a Round_Box from shapes.inc without the curves on z=0
    // bulges on the front
    #if (NumBulges > 0)
      #local BulgeArray = array[NumBulges*2];
      #local BulgeArrayZ = array[NumBulgesZ*2];
      RandArray(BulgeArray, corner2.x - Rsmall*2)
      #for (I, 0, NumBulges-1)
        #local BulgeStart = BulgeArray[I * 2] + Rsmall;
        #local BulgeStop = BulgeArray[I * 2 + 1] + Rsmall;
        cylinder {0, (BulgeStop-BulgeStart) * x, Rsmall*1.1 translate <BulgeStart, corner2.y-Rsmall, Rsmall>}
        sphere {<BulgeStart, corner2.y-Rsmall, Rsmall>, Rsmall*1.1}
        sphere {<BulgeStop, corner2.y-Rsmall, Rsmall>, Rsmall*1.1}
      #end
      // bulges on the back
      RandArray(BulgeArray, corner2.x - Rsmall*2)
      #for (I, 0, NumBulges-1)
        #local BulgeStart = BulgeArray[I * 2] + Rsmall;
        #local BulgeStop = BulgeArray[I * 2 + 1] + Rsmall;
        cylinder {0, (BulgeStop-BulgeStart) * x, Rsmall*1.1 translate <BulgeStart, corner2.y-Rsmall, ZZ>}
        sphere {<BulgeStart, corner2.y-Rsmall, ZZ>, Rsmall*1.1}
        sphere {<BulgeStop, corner2.y-Rsmall, ZZ>, Rsmall*1.1}
      #end
      // bulges on the left
      RandArray(BulgeArrayZ, corner2.z - Rsmall*2)
      #for (I, 0, NumBulgesZ-1)
        #local BulgeStart = BulgeArrayZ[I * 2] + Rsmall;
        #local BulgeStop = BulgeArrayZ[I * 2 + 1] + Rsmall;
        cylinder {0, (BulgeStop-BulgeStart) * z, Rsmall*1.1 translate <Rsmall, YY, BulgeStart>}
        sphere {<Rsmall, YY, BulgeStart>, Rsmall*1.1}
        sphere {<Rsmall, YY, BulgeStop>, Rsmall*1.1}
      #end
      // bulges on the right
      RandArray(BulgeArrayZ, corner2.z - Rsmall*2)
      #for (I, 0, NumBulgesZ-1)
        #local BulgeStart = BulgeArrayZ[I * 2] + Rsmall;
        #local BulgeStop = BulgeArrayZ[I * 2 + 1] + Rsmall;
        cylinder {0, (BulgeStop-BulgeStart) * z, Rsmall*1.1 translate <XX, YY, BulgeStart>}
        sphere {<XX, YY, BulgeStart>, Rsmall*1.1}
        sphere {<XX, YY, BulgeStop>, Rsmall*1.1}
      #end
    #end
  } // end positive objects.  Now chip away at it
  #for (I, 1, NumChipsX)
    cylinder {-Rsmall * 3 * z, Rsmall * 3 * z, Rsmall
      scale <2, 1, 1>
      translate Rsmall * 0.3 * y // lessen the impact
      rotate -(30+rand(rseed)*30)*x 
      translate <rand(rseed) * corner2.x, corner2.y, 0>
    }
  #end // for, chips
  bounded_by {box{0, corner2}}  
}


//  superellipsoid { <0.1, 0.1>     translate 1
//    scale <corner2.x-corner1.x, corner2.y-corner1.y, corner2.z-corner1.z> / 2
//    translate corner1   }

#end // roughbox macro

//=====================================================
#macro roughcylinder (end1, end2, R, num_chips, NumBulges)

#if (NumBulges > 0)
# local BulgeArray = array[NumBulges*2];
  RandArray(BulgeArray, 360)
#end

#if (num_chips > 0) difference #else object #end
{
//union{
  merge {
    //Round_Cylinder_Tube(end1, end2, R, R/10, 1, 1) minus the bottom
    cylinder{0, (vlength(end2-end1) - R/10)*y, R}
    cylinder{0, vlength(end2-end1)*y, R*0.9}
    torus{R*0.9, R/10 translate (vlength(end2-end1) - R/10)*y}
    //, R/10, 1, 1)
    // a bunch of bulges around the top of the cylinder
    #if (NumBulges > 0)
      merge {
        #for (I, 0, NumBulges-1)
          #local ArcStart = BulgeArray[I * 2];
          #local ArcAngle = BulgeArray[I * 2 + 1] - ArcStart;
          object { Segment_of_Torus(R*0.9, R*0.11, ArcAngle) rotate ArcStart*y}
          // A sphere at each end to make our bulge smooth
          sphere { R*0.9*x, R*0.11 rotate ArcStart * y}
          sphere { R*0.9*x, R*0.11 rotate (ArcStart + ArcAngle) * y}
        #end
        translate (vlength(end2 - end1) - R/10) * y // thanks shapes3.inc
      }  // merge of bulges
    #end // #if any bulges
  } // end merge of additive objects.  Now subtract:
  #for (I, 1, num_chips, 1)
    #if (rand(rseed) > 0.5)
      box {<-R/2, 0, 0>, <R/2, R/10, R/10>
        rotate (10 + rand(rseed) * 70) *x
    #else
      cylinder {<-R/2, 0, 0>, <R/2, 0, 0>, R/20
      scale <1, 1, 2> // widen
    #end
      translate (R*0.13) * y // get centre of object onto surface of the radius we will cut
      rotate (20 + rand(rseed) * 50) * -z // tilt
      #local Rot = rand(rseed) * 360;
      rotate (30) * y // as though the blow came from an angle
      translate (R-R/10) * x // move to the +x edge of the cylinder
      rotate Rot * y // move around the edge of the cylinder
      translate (vlength(end2 - end1) - R/10) * y // thanks shapes3.inc
    }
  #end // for chips
  bounded_by {cylinder {0, vlength(end2-end1)*y, R}}
  Point_At_Trans(end2 - end1)
  translate end1
  normal { dents R/5 scale R/10 }

} // end difference/object
#end // roughbox macro

// =======================================================
// gappywall
// c2.x must be longest dimension, because it gets the gaps.
// c2.y must be height of wall.
// Leaving c2.z for the thickness.
// MaxGapDepth is a fraction of the wall's height.
#macro gappywall (c2, NumGaps, MinGapDepth, MaxGapDepth)

#if (MinGapDepth < 0 | MinGapDepth > 1 | MaxGapDepth < 0 | MaxGapDepth > 1 | MinGapDepth > MaxGapDepth)
#error "MinGapDepth out of range"
#end

#local GapArray = array[NumGaps*2];
RandArray(GapArray, c2.x*0.8)

difference {
  box {0, c2}
  #for (I, 0, NumGaps-1)
    box {<GapArray[I*2] + c2.x*0.1, c2.y*(1-rand(rseed)*(MaxGapDepth-MinGapDepth)-MinGapDepth), -E>
         <GapArray[I*2+1] + c2.x*0.1, c2.y+E, c2.z+e>
    }
  #end // for
  }

#end // gappywall macro


// =======================================================
// roughgappywall
// c2.x must be longest dimension, because it gets the gaps.
// c2.y must be height of wall.
// Leaving c2.z for the thickness.
// MaxGapDepth is a fraction of the wall's height.
#macro roughgappywall (c2, RSmall, NumGaps, MinGapDepth, MaxGapDepth, ChipsPerUnit, BulgesPerUnit)

#if (MinGapDepth < 0 | MinGapDepth > 1 | MaxGapDepth < 0 | MaxGapDepth > 1 | MinGapDepth > MaxGapDepth)
#error "Bad gap depths"
#end

#local HChangeArray = array[NumGaps*2 + 1];
RandLimitedGapArray(HChangeArray, c2.x*0.8, 0.08)
#local HChangeArray[NumGaps*2] = c2.x*0.9;

union{
  object{roughbox(<HChangeArray[0] + c2.x*0.1 + RSmall, c2.y, c2.z>,
                RSmall,
                ChipsPerUnit*(HChangeArray[0] + c2.x*0.1),
                BulgesPerUnit*(HChangeArray[0] + c2.x*0.1))}
  #for (I, 0, NumGaps*2-1)
  object {
    roughbox(<HChangeArray[I+1] - HChangeArray[I] + RSmall*2,
              #if (mod(I,2) = 0)
                c2.y*(1-rand(rseed)*(MaxGapDepth-MinGapDepth)-MinGapDepth),
              #else
                c2.y,
              #end
              c2.z>, RSmall,
              ChipsPerUnit*(HChangeArray[I+1] - HChangeArray[I]),
              BulgesPerUnit*(HChangeArray[I+1] - HChangeArray[I]))
    translate (HChangeArray[I]+c2.x*0.1 - RSmall) * x
  }
  #end // for
  // normal {brick 0.5 brick_size <0.1,0.1,1>}
}
#end // gappywall macro

