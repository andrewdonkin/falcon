#version 3.7;
#include "math.inc"
#include "stdafx.inc"
#default {
  texture {
    pigment { rgb 1 }
    //normal { bumps 1/20 scale 1/100 }
    finish { ambient 0.1 reflection 0.1 phong 1 }
  }
}

global_settings {
   ambient_light 1
  //ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

light_source { <8, 8, -40> rgb 1 }

//sky_sphere {
  // pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
  //pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
  //pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
//}

plane {y, 0
        texture {
              pigment{ color rgb <1,1,1 >*0.85 } 
              finish { phong 1 reflection 0.4 } 
        } // end of texture      
}  

// #declare CHEAP=1;
#declare DEBUG=1;
#include "hexdron.inc"

#macro cornerball (C, V1, V2, V3, R)
  #local N31=vcross(V3-C, V1-C);
  #local N23=vcross(V2-C, V3-C);
  //#local Bvec=vnormalize(vcross(N23, N31));
  #local CA1=vnormalize(V1-C + V2-C);
  #local N12=vcross(V1-C, V2-C);
  #local A3=R*vnormalize(N12);
  #local V3C1=vnormalize(V3-C);
  #local Ang3=VAngle(V3-C, N12);
  //#local Ang = VAngle(V1-C, V2-C) / 2;
  #local B3=(V3C1 * R / cos(Ang3));
  // result is C + B3 + ? * CA
  #local Ang=(180-VAngle(N31, N23))/2;

  
  #ifdef (DEBUG)
  triangle {V1, C, V2}
  triangle {V1, C, V3}
  triangle {V2, C, V3}
  sphere {(V1+V3)/2, 0.015 pigment {rgb <0,1,1>}}
  cylinder {(V1+V3)/2, (V1+V3)/2 + N31/2, 0.01 pigment {rgb <0,1,1>}}
  sphere {(V2+V3)/2, 0.015 pigment {rgb <1,1,0>}}
  cylinder {(V2+V3)/2, (V2+V3)/2 + N23/2, 0.01 pigment {rgb <1,1,0>}}
  sphere {(V1+V2)/2, 0.015 pigment {rgb <1,0.5,0>}}
  cylinder {(V1+V2)/2, (V1+V2)/2 + N12/2, 0.01 pigment {rgb <1,0.5,0>}}
  sphere {C+B3, 0.01 pigment {rgb <1,0,1>}}
  cylinder {C+B3, C+B3 + CA1, 0.01 pigment {rgb <1,0,1>}}
  sphere {C+B3 + R/sin(Ang)*CA1, R*1.01 pigment {rgb <1,0,0>}}
  #end
  
//  sphere {C+B, R/2 pigment {rgb <0,1,0>}}
#end

#local TLF=<0, 1, 0>;
#local TLR=<0, 1, 1>;
#local TRR=<1, 1, 1>;
#local TRF=<1, 1, 0>;
#local BLF=-0.4;
#local BLR=<0, 0, 1>;
#local BRR=<1, 0, 1>;
#local BRF=<1, 0, 0>;

union {

//  hexdron(TLF, TLR, <1, 1, 1>, TRF,
//          BLF, BLR, <1, 0, 1>, BRF,
//           0  // edge radius
//           , 0 // use saddle?
//  )
  cylinder{BLF, TLF, 1/100} // cylinder{BLR, TLR, 1/100}
  //cylinder{BRR, TRR, 1/100} cylinder{BRF, TRF, 1/100}
  cylinder{BLF, BLR, 1/100} //cylinder{BLR, BRR, 1/100}
  cylinder{BRF, BLF, 1/100} //cylinder{BRR, BRF, 1/100}
  //cylinder{TLF, TLR, 1/100} cylinder{TLR, TRR, 1/100}
  //cylinder{TRR, TRF, 1/100} cylinder{TRF, TLF, 1/100}

  #local R=0.1001;
  cornerball(BLF, BLR, BRF, TLF, R)
  //sphere {cornerball(BLF, BLR, BRF, TLF, R), R pigment {rgb x}}

  rotate <50, -30, 0>
  texture {
    pigment {rgbf <1, 1, 1, 0.5>}
    finish { ambient 0.1 reflection 0.0 specular 0.1 roughness 0.0001}
  }
  translate y
}

camera { location <0, 2, -8> look_at y angle 17}

