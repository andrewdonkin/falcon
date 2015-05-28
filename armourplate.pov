global_settings {
 //  ambient_light 0
  // ambient_light 0  radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

plane { y, 0
  pigment {
    checker color rgb <0,0,0>, colour rgb <1,1,1>
  }
}

#local bigrad = 77.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.1;
#local tinyrad=thick/4;

#local rotwidth = 1;
#local rotdepth = 1.3;

#local xclippers = union {
      plane {  z 0               texture { pigment { color rgb<0,1,1> } } }
      plane { -z 0 rotate  rotdepth * x texture { pigment { color rgb<0,1,1> } } }
      };
#local zclippers = union {
      plane {  x 0               texture { pigment { color rgb<1,0,1> } } }
      plane { -x 0 rotate -rotwidth * z texture { pigment { color rgb<1,0,1> } } }
      };


union {
  union {
    difference { // quadrilateral cut out of a ball, sharp edges
      sphere { 0, bigrad + thick }
      sphere { 0, bigrad }
      xclippers
      zclippers
    }
    difference { // rolled-off edges on constant x
      union {
        torus {bigrad+thick-tinyrad, tinyrad rotate 90*z}
        torus {bigrad+thick-tinyrad, tinyrad rotate (90-rotwidth)*z}
      }
      xclippers
    }
    difference { // rolled-off edges on constant z
      union {
        torus {bigrad+thick-tinyrad, tinyrad rotate (90+rotdepth)*x}
        torus {bigrad+thick-tinyrad, tinyrad rotate 90*x}
      }
      zclippers
    }
  }
  // four balls to join the rolled-off edges
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y rotate -rotwidth * z}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y rotate  rotdepth * x}
  sphere { 0 tinyrad translate (bigrad + thick - tinyrad) * y rotate <rotdepth, 0, -rotwidth>}
  
  // larger quadrilateral cut out of a ball, smaller radius
  #local rotdelta = degrees(asin (tinyrad / (bigrad - tinyrad)));
  difference {
    sphere { 0, bigrad + thick - tinyrad }
    sphere { 0, bigrad }
    plane {  x 0 rotate  rotdelta * z }
    plane { -x 0 rotate -(rotwidth + rotdelta) * z }
    xclippers
  }
  difference {
    sphere { 0, bigrad + thick - tinyrad }
    sphere { 0, bigrad }
    plane {  z 0 rotate -rotdelta * x }
    plane { -z 0 rotate  (rotdepth + rotdelta) * x}
    zclippers
  }
  // four little posts, along big radii, that form the rounded-off "vertical" corners of our tile
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad }
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate -rotwidth * z}
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate  rotdepth * x}
  cylinder { (bigrad) * y, (bigrad + thick - tinyrad) * y, tinyrad rotate <rotdepth, 0, -rotwidth>}
  
  texture { pigment { color rgb<1,1,1> }}  
  translate -(bigrad - 1 ) * y
}


union {// a few stakes 
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, 0> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <       0, 0, -rotwidth> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, -rotwidth> }
  translate -(bigrad - 1) * y
  texture { pigment { color rgb<1,1,0> }}
}



#if (0)
light_source {
  <0,0,0>             // light's position (translated below)
  color rgb 1.0       // light's color
  area_light
  <0.5, 0, 0> <0, 0.5, 0>
  9, 9
  adaptive 1          // 0,1,2,3...
  //jitter              // adds random softening of light
  circular            // make the shape of the light circular
   orient              // orient light
    looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 diffuse 1 }}}
  translate <2, 1.5, -2>
}
#end

light_source { <2, 1.5, -2> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}


camera { location <1, 3, -3> look_at <0, 1, 0.5> }
