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

  #warning concat("cornerball C:",
    str(C.x,1,1), ",", str(C.y,1,1), ",",str(C.z,1,1),
    " V1: ", str(V1.x,1,1), ",", str(V1.y,1,1), ",",str(V1.z,1,1),
    " V2: ", str(V2.x,1,1), ",", str(V2.y,1,1), ",",str(V2.z,1,1),
    " V3: ", str(V3.x,1,1), ",", str(V3.y,1,1), ",",str(V3.z,1,1)
    )

  #local (P1, D1) = PlaneFrom3Points(C, V1, V2);
  #local (P2, D2) = PlaneFrom3Points(C, V3, V1);
  #local (P3, D3) = PlaneFrom3Points(C, V2, V3);

  #local D1 = D1 - R;   #local D2 = D2 - R;   #local D3 = D3 - R;
  #local Det=Matrix3determinated(P1.x,P1.y,P1.z, P2.x,P2.y,P2.z, P3.x,P3.y,P3.z);
  #if (Det = 0)
  #error concat("pffffpfpfpf Det = 0 C:",
    str(C.x,1,2), ",", str(C.y,1,2), ",",str(C.z,1,2),
    " V1: ", str(V1.x,1,2), ",", str(V1.y,1,2), ",",str(V1.z,1,2),
    " V2: ", str(V2.x,1,2), ",", str(V2.y,1,2), ",",str(V2.z,1,2))
  #end
  #local X = Matrix3determinated(D1,P1.y,P1.z, D2,P2.y,P2.x, D3,P3.y,P3.z) / Det;
  #local Y = Matrix3determinated(P1.x,D1,P1.z, P2.x,D2,P2.x, P3.x,D3,P3.z) / Det;
  #local Z = Matrix3determinated(P1.x,P1.y,D1, P2.x,P2.y,D2, P3.x,P3.y,D3) / Det;

  #ifdef (DEBUG)
  intersection {plane {P1, D1} plane {P2, D2} plane {P3, D3}}

  #warning concat("P1:", str(P1.x,1,2), ",", str(P1.y,1,2), ",",str(P1.z,1,2), ",", str(D1, 1, 2))
  #warning concat("P2:", str(P2.x,1,2), ",", str(P2.y,1,2), ",",str(P2.z,1,2), ",", str(D2, 1, 2))
  #warning concat("P3:", str(P3.x,1,2), ",", str(P3.y,1,2), ",",str(P3.z,1,2), ",", str(D3, 1, 2))

  #warning concat("Det = ", str(Det, 1, 2), " C:",
    str(C.x,1,2), ",", str(C.y,1,2), ",",str(C.z,1,2),
    " V1: ", str(V1.x,1,2), ",", str(V1.y,1,2), ",",str(V1.z,1,2),
    " V2: ", str(V2.x,1,2), ",", str(V2.y,1,2), ",",str(V2.z,1,2))

  #warning concat("XYZ ", str(X, 1, 4), " ", str(Y, 1, 4), " ", str(Z, 1, 4))
 
  disc {(V1+V2)/2, P1, 0.1 pigment {rgb <1,0.5,0>}}
  cylinder {(V1+V2)/2, (V1+V2)/2 + vnormalize(P1)/2, 0.01 pigment {rgb <1,0.5,0>}}
  disc {(V1+V3)/2, P2, 0.1 pigment {rgb <0,1,1>}}
  cylinder {(V1+V3)/2, (V1+V3)/2 + vnormalize(P2)/2, 0.01 pigment {rgb <0,1,1>}}
  disc {(V2+V3)/2, P3, 0.1 pigment {rgb <1,1,0>}}
  cylinder {(V2+V3)/2, (V2+V3)/2 + vnormalize(P3)/2, 0.01 pigment {rgb <1,1,0>}}
  sphere { <X, Y, Z>, R*1.001 pigment {rgb <0,0,0>}}
  triangle {V1, C, V2}
  triangle {V1, C, V3}
  triangle {V2, C, V3}
  #end
  
//  sphere {C+B, R/2 pigment {rgb <0,1,0>}}
  <X, Y, Z>
#end

#local TLF=<1, 2, 1>;
#local TLR=<1, 2, 2>;
#local TRR=<2, 2, 2>;
#local TRF=<2, 2, 1>;
#local BLF=<0.1, 1, 1>;
#local BLR=<1, 1, 2>;
#local BRR=<2, 1, 2>;
#local BRF=<2, 1, 1>;

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

  #local R=0.100;
  cornerball(BLF, BLR, TLF, BRF, R)
  //sphere {cornerball(BLF, BLR, BRF, TLF, R), R pigment {rgb x}}

  translate <-1, -1, -1>
  //rotate <50, -30, 0>
  rotate <0, -70, 0>
  rotate <20, 0, 0>
  texture {
    pigment {rgbf <1, 1, 1, 0.5>}
    finish { ambient 0.1 reflection 0.0 specular 0.1 roughness 0.0001}
  }
  translate y
}

camera { location <0, 2, -8> look_at y angle 27}

