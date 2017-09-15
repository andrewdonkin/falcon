#include "shapes3.inc"

global_settings {
 //  ambient_light 0
  // ambient_light 0  radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

plane { y, 0
  pigment {
    checker color rgb 0.2, colour rgb <1,1,1>
  }
}

#local bigrad = 77.48 ; // big radius.  Not bi grad.  77.48
#local thick = 0.1;
#local rotwidth = 1;
#local rotdepth = 1.3;

union {// a few stakes 
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, 0> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <       0, 0, -rotwidth> }
  cylinder { (bigrad - 1) * y, (bigrad + 2*thick)*y, 0.01 rotate <rotdepth, 0, -rotwidth> }
  translate -(bigrad - 1) * y
  texture { pigment { color rgb<1,1,0> }}
}


union {
  sphere {0, thick translate <0, thick, 0>}
  cylinder {0, z, thick
    translate <0, 0, 1>}

  object{ Segment_of_Torus(1, thick, -45) rotate <0,90,0> translate <thick, 0, 1> rotate <0,0,90>}

  texture { pigment { 
   color rgb<1,1,0> 
    }}
}

// one way of merging a block into the plane
#local R=0.1;
isosurface  {
  function {
    (1+0.01)
     - pow(0.01, f_rounded_box(x,y,z, 0.1, 1, 1, 1)
     - pow(0.00001, (y+0.2))
     )
  }
  //function { -f_superellipsoid(x,y,z,0.08, 0.08) }
  contained_by {box {<-2, 0, -2>,
                     <2, 1, 2>}}
  evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
  accuracy 0.001*R // default is fine until you stitch them together
  scale 0.5
    translate <-0.25, 0, 0.5>
  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
}

// another way: smooth transient between the plane and the box skirting
#local R=0.1;
union {
  isosurface  {
    function {f_rounded_box(x,y,z, 0.1, 1, 1, 1)}
    contained_by {box {<-1, 0, -1>,
                       <1, 1, 1>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
  }
  difference {
    merge {
      // these four cylinders lying at the base of the box
      // will get another cylinder subtracted later
      // to form a radiused skirting.
      cylinder {<-1+R, 0, -1>, <1-R, 0, -1>, 0.1} // near
      cylinder {<-1, 0, -1+R>, <-1, 0, 1-R>, 0.1} //left 
      cylinder {<-1+R, 0, 1>, <1-R, 0, 1>, 0.1} // rear
      cylinder {<1, 0, -1+R>, <1, 0, 1-R>, 0.1} // right
      
      // these four hockey pucks will have quarter of a torus taken out of them
      cylinder {<-1+R, 0, -1+R>, <-1+R, R, -1+R>, R*2} // near left
      cylinder {<1-R,  0, -1+R>, <1-R,  R, -1+R>, R*2} // near right
      cylinder {<-1+R, 0,  1-R>, <-1+R, R,  1-R>, R*2} // far left
      cylinder {<1-R,  0,  1-R>, <1-R,  R,  1-R>, R*2} // far right
    }
    cylinder {<-1, R, -1-R>, <1, R, -1-R>, R} // near
    cylinder {<-1-R, R, -1>, <-1-R, R, 1>, R} // left
    cylinder {<-1, R, 1+R>, <1, R, 1+R>, R} // rear
    cylinder {<1+R, R, -1>, <1+R, R, 1+R>, R} //right
    object {Segment_of_Torus(R*2, R, 90) rotate 90*y translate <-1+R, R, -1+R> } // near left
    object {Segment_of_Torus(R*2, R, 90) translate <1-R, R, -1+R> } // near right
    object {Segment_of_Torus(R*2, R, 90) rotate 180*y translate <-1+R, R, 1-R> } // far left
    object {Segment_of_Torus(R*2, R, -90) translate <1-R, R, 1-R> } // far right

  }
  
  
  scale 0.5
  rotate 180*y
  translate <1.25, 0, 0.5>
  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
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
#else

light_source { <2, 1.5, -2> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}
#end

camera { location <0, 3, -3> look_at <1, 0, 0> angle 40}
camera { location <4, 3, -1> look_at <1.5, 0, 1> angle 30}
//camera { location <0, 3, -1> look_at <1, 0, 1> angle 30}
