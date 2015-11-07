// Author:  Andrew Donkin
//
// This file contains macros for creating smoothed and roughed-up geometric shapes.
//

#include "shapes3.inc"
#include "arrays.inc"

#declare rseed = seed(1);

//=====================================================
#macro roughbox (corner2, Rsmall, NumChipsX, NumBulges)

//  #local num_chips_y = abs(corner2.x) / num_chips_x * abs(corner2.y);
//  #local num_chips_z = abs(corner2.x) / num_chips_x * abs(corner2.z);

#local NumBulgesZ = ceil(abs(NumBulges / corner2.x * corner2.z));

#local XX=corner2.x-Rsmall;
#local YY=corner2.y-Rsmall;
#local ZZ=corner2.z-Rsmall;

#local BulgeArray = array[NumBulges*2];
#local BulgeArrayZ = array[NumBulgesZ*2];

difference {
  union {
    box {<Rsmall, 0, Rsmall>, <corner2.x - Rsmall, corner2.y, corner2.z - Rsmall>} //tall
    box {<0, 0, Rsmall>, <corner2.x, corner2.y - Rsmall, corner2.z - Rsmall>} // wide
    box {<Rsmall, 0, 0>, <corner2.x - Rsmall, corner2.y - Rsmall, corner2.z>} // deep
    cylinder {0, <0, corner2.y - Rsmall, 0>, Rsmall translate <Rsmall, 0, Rsmall>} // front left vertical
    cylinder {0, <0, corner2.y - Rsmall, 0>, Rsmall translate <Rsmall, 0, corner2.z-Rsmall>} // back left vert
    cylinder {0, <0, corner2.y - Rsmall, 0>, Rsmall translate <corner2.x-Rsmall, 0, Rsmall>} // front right vert
    cylinder {0, <0, corner2.y - Rsmall, 0>, Rsmall translate <corner2.x-Rsmall, 0, corner2.z-Rsmall>} // back right vert
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
    #for (I, 0, NumBulges * 2 - 1, 1)
      #local BulgeArray[I] = rand(rseed) * (corner2.x - Rsmall*2);
    #end // for
    Sort_Array(BulgeArray)
    #for (I, 0, NumBulges-1)
      #local BulgeStart = BulgeArray[I * 2] + Rsmall;
      #local BulgeStop = BulgeArray[I * 2 + 1] + Rsmall;
      cylinder {0, (BulgeStop-BulgeStart) * x, Rsmall*1.1 translate <BulgeStart, corner2.y-Rsmall, Rsmall>}
      sphere {<BulgeStart, corner2.y-Rsmall, Rsmall>, Rsmall*1.1}
      sphere {<BulgeStop, corner2.y-Rsmall, Rsmall>, Rsmall*1.1}
    #end
    // bulges on the back
    #for (I, 0, NumBulges * 2 - 1, 1)
      #local BulgeArray[I] = rand(rseed) * (corner2.x - Rsmall*2);
    #end // for
    Sort_Array(BulgeArray)
    #for (I, 0, NumBulges-1)
      #local BulgeStart = BulgeArray[I * 2] + Rsmall;
      #local BulgeStop = BulgeArray[I * 2 + 1] + Rsmall;
      cylinder {0, (BulgeStop-BulgeStart) * x, Rsmall*1.1 translate <BulgeStart, corner2.y-Rsmall, ZZ>}
      sphere {<BulgeStart, corner2.y-Rsmall, ZZ>, Rsmall*1.1}
      sphere {<BulgeStop, corner2.y-Rsmall, ZZ>, Rsmall*1.1}
    #end
    // bulges on the left
    #for (I, 0, NumBulgesZ * 2 - 1, 1)
      #local BulgeArrayZ[I] = rand(rseed) * (corner2.z - Rsmall*2);
    #end // for
    Sort_Array(BulgeArrayZ)
    #for (I, 0, NumBulgesZ-1)
      #local BulgeStart = BulgeArrayZ[I * 2] + Rsmall;
      #local BulgeStop = BulgeArrayZ[I * 2 + 1] + Rsmall;
      cylinder {0, (BulgeStop-BulgeStart) * z, Rsmall*1.1 translate <Rsmall, YY, BulgeStart>}
      sphere {<Rsmall, YY, BulgeStart>, Rsmall*1.1}
      sphere {<Rsmall, YY, BulgeStop>, Rsmall*1.1}
    #end
    // bulges on the right
    #for (I, 0, NumBulgesZ * 2 - 1, 1)
      #local BulgeArrayZ[I] = rand(rseed) * (corner2.z - Rsmall*2);
    #end // for
    Sort_Array(BulgeArrayZ)
    #for (I, 0, NumBulgesZ-1)
      #local BulgeStart = BulgeArrayZ[I * 2] + Rsmall;
      #local BulgeStop = BulgeArrayZ[I * 2 + 1] + Rsmall;
      cylinder {0, (BulgeStop-BulgeStart) * z, Rsmall*1.1 translate <XX, YY, BulgeStart>}
      sphere {<XX, YY, BulgeStart>, Rsmall*1.1}
      sphere {<XX, YY, BulgeStop>, Rsmall*1.1}
    #end
  }
  
}


//  superellipsoid { <0.1, 0.1>     translate 1
//    scale <corner2.x-corner1.x, corner2.y-corner1.y, corner2.z-corner1.z> / 2
//    translate corner1   }

#end // roughbox macro

//=====================================================
#macro roughcylinder (end1, end2, R, num_chips)

#local NumBulges = floor(num_chips / 10);

#local BulgeArray = array[NumBulges*2];
#for (I, 0, NumBulges * 2 - 1, 1)
  #local BulgeArray[I] = rand(rseed) * 360;
#end // if
Sort_Array(BulgeArray)

difference {
  merge {
    //Round_Cylinder_Tube(end1, end2, R, R/10, 1, 1) minus the bottom
    cylinder{0, (vlength(end2-end1) - R/10)*y, R}
    cylinder{0, vlength(end2-end1)*y, R*0.9}
    torus{R*0.9, R/10 translate (vlength(end2-end1) - R/10)*y}
    //, R/10, 1, 1)
    // a bunch of bulges around the top of the cylinder
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
  } // merge of additive objects.  Now subtract:
  #for (I, 1, num_chips, 1)
    #if (rand(rseed) > 0.5)
      box {<-R/2, 0, 0>, <R/2, R/10, R/10>
        rotate (10 + rand(rseed) * 70) *x
    #else
      cylinder {<-R/2, 0, 0>, <R/2, 0, 0>, R/20
    #end
      translate (rand(rseed)*0.01 -0.005) * y
      rotate (20 + rand(rseed) * 50) * -z
      rotate (20 + rand(rseed) * 50) * y
      translate R * x
      rotate rand(rseed) * 360 * y
      translate vlength(end2 - end1) * y // thanks shapes3.inc
    }
  #end // for chips
  Point_At_Trans(end2 - end1)
  translate end1
} // end difference
#end // roughbox macro
