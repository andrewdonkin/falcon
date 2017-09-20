#include "shapes3.inc" // f_rounded_box()
#include "skirtbox.inc"

global_settings {
 //  ambient_light 0
  // ambient_light 0  radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

#local floor_pig = pigment {
    checker color rgb <0.3, 0.3, 1>, colour rgb <0.8,0.8,1>
  };

plane { y, 0
  pigment {
    floor_pig
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
#macro skirted_box2(Box
                    , Rbox  //
                    , skirting // (0, 1].  1 is good.  Smaller=tighter
                    )
  #local Boxx=Box.x; // functions don't like vectors.
  #local Boxy=Box.y;
  #local Boxz=Box.z;
  isosurface  {
    function {
      (1+0.001)
       - pow(0.0001, f_rounded_box(x-Boxx/2,y,z-Boxz/2, Rbox, Boxx/2, Boxy/2, Boxz/2)
       //-f_superellipsoid(x*2-Boxx, y*2, z*2-Boxz,0.08, 0.08)
       )
       - pow(0.00001 * skirting, (y+Boxy/20))
    }
    contained_by {box {<-Boxy/3, 0, -Boxy/3>,
                       <Boxx+Boxy/3, Boxy/2, Boxz+Boxy/3>}}
    evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
    accuracy 0.001*Rbox // default is fine until you stitch them together
  }
#end

object {
  skirted_box2(<1, 1, 1>, 0.03, 1)
  translate <0.25, 0, 0>
  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
}

// another way: smooth transient between the plane and the box skirting
#macro skirted_box(Box                 // 3-vector of box dimensions
                   , Rbox          // radius of box's edges
                   , R)           // radius of skirting
  #local Boxx=Box.x; // functions don't like vectors.
  #local Boxy=Box.y;
  #local Boxz=Box.z;
  union {
    isosurface  {
      function {f_rounded_box(x-Boxx/2,y,z-Boxz/2, Rbox, Boxx/2, Boxy/2, Boxz/2)
        // + f_noise3d(x*10, y*20, z*20)/40
        }
      contained_by {box {<0, 0, 0>,
                         <Boxx, Boxy/2, Boxz>}}
      evaluate 1*0.6,  sqrt(1/(1*0.6)),  0.7
      accuracy 0.000001
    }
    difference {
      merge {
        // these four cylinders lying at the base of the box
        // will get another cylinder subtracted later
        // to form a radiused skirting.
        cylinder {<Rbox, 0, 0>, <Boxx-Rbox, 0, 0>, R} // near
        cylinder {<0, 0, Rbox>, <0, 0, Boxz-Rbox>, R} //left 
        cylinder {<Rbox, 0, Boxz>, <Boxx-Rbox, 0, Boxz>, R} // rear
        cylinder {<Boxx, 0, Rbox>, <Boxx, 0, Boxz-Rbox>, R} // right
        
        // these four hockey pucks will have quarter of a torus taken out of them
        cylinder {<+Rbox, 0, Rbox>, <Rbox, R, Rbox>, R+Rbox} // near left
        cylinder {<Boxx-Rbox,  0, Rbox>, <Boxx-Rbox,  R, Rbox>, R+Rbox} // near right
        cylinder {<+Rbox, 0,  Boxz-Rbox>, <+Rbox, R,  Boxz-Rbox>, R+Rbox} // far left
        cylinder {<Boxx-Rbox,  0,  Boxz-Rbox>, <Boxx-Rbox,  R,  Boxz-Rbox>, R+Rbox} // far right
      }
      cylinder {<0, R, -R>, <Boxx, R, -R>, R} // near
      cylinder {<-R, R, -Boxz>, <-R, R, Boxz>, R} // left
      cylinder {<0, R, Boxz+R>, <Boxx, R, Boxz+R>, R} // rear
      cylinder {<Boxx+R, R, -Boxz>, <Boxx+R, R, Boxz+R>, R} //right
      object {Segment_of_Torus(R+Rbox, R, 90) rotate 90*y translate <Rbox, R, Rbox> } // near left
      object {Segment_of_Torus(R+Rbox, R, 90) translate <Boxx-Rbox, R, Rbox> } // near right
      object {Segment_of_Torus(R+Rbox, R, 90) rotate 180*y translate <Rbox, R, Boxz-Rbox> } // far left
      object {Segment_of_Torus(R+Rbox, R, -90) translate <Boxx-Rbox, R, Boxz-Rbox> } // far right
    }
  }
#end

object {
  skirted_box(<1, 1, 1>, 0.03, 0.02)
  translate <1.5, 0, 0>
  texture { pigment { checker color rgb 0.2, colour rgb <1,1,1> }}
  }

object {
  skirted_box_y(<0, 0, -1>, <1, 0.4, -0.5>,
    0.2, 0.03, 0.02, 0) // big rad, top rad, skirt rad, filled
  texture { floor_pig}
}

object {
  skirted_lozenge_y(0, 1, 0.5, 0.2, // end rad
    0.03, // top rad
    0.02, // skirt
    0) // filled
  rotate -90*y
  translate <2.5, 0, -1>
  texture { floor_pig }
  
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

camera { location <-0.1, 3, -4> look_at <1, 0, -0.5> angle 40}
//camera { location <-0.1, 3, -4> look_at <2, 0, -0.9> angle 5}
