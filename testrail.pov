#version 3.7;
#include "stdafx.inc"
#include "functions.inc"
//#local Eps=0.00001;
global_settings {
  ambient_light 0  radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

#local Crack = function {-pow(f_crackle(x*3,y,z*3), 8)};
#local floor_tex = texture {
  pigment {checker color rgb <0.3, 0.3, 1>, colour rgb <0.8,0.8,1>}
  finish { phong 1 reflection 0.1*0 }
//  normal { function {Crack(x,y,z)} 0.2}
};

plane { y, 0 texture { floor_tex } }

#if (1)
light_source {
  <0,0,0>             // light's position (translated below)
  color rgb 500.0       // light's color
  //area_light  <0.4, 0, 0> <0, 0.4, 0>  18, 18
  //adaptive 1          // 0,1,2,3...
  //jitter              // adds random softening of light
  //circular            // make the shape of the light circular
  // orient              // orient light
  fade_power 2
  fade_distance 1
  //  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 diffuse 1 }}}
  translate <25, 18, -25>
}
#else

light_source { <2, 1.5, -15> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}
#end

camera { location <0, 20, -20> look_at <0, 5, 1> angle 60}
//camera { location <18, 6, -4> look_at <0,4, 0> angle 15} //end of rail
//camera { location <4, 10, -20> look_at <4, 1.5, 1> angle 10} // checking coincidents on slot

// Extends fractionally beyond Z, for use in subtractive CSG.
// That can mess up textures if the object is visible, though.
#macro Spool( Rmaj, Rmin, Z )
  difference {
    union {
      cylinder { <0,0,-Eps>, <0,0, Z+Eps>, Rmaj }
      cylinder { <0,0,-Eps>, <0,0, Rmin>, Rmaj+Rmin }
      cylinder { <0,0,Z-Rmin>, <0,0, Z+Eps>, Rmaj+Rmin }
    }
    torus {Rmaj+Rmin, Rmin rotate 90*x translate <0,0,Rmin> }
    torus {Rmaj+Rmin, Rmin rotate 90*x translate <0,0,Z-Rmin> }
  }
#end

// Y parameter includes the lip.  If you use this to cut a slot,
// the slot will be (Y-2.Rmin) in length.
// Extends fractionally beyond Z, for use in subtractive CSG.
// That can mess up textures if the object is visible, though.
#macro LongSpool( Rmaj, Rmin, Z, Y )
  #if (Y < 2 * (Rmaj + Rmin)) #error "undersized spool" #end
  #if (Y = 2 * (Rmaj + Rmin))
    Spool(Rmaj, Rmin, Z);
  #else
    difference {
      union {
        #local End = object {Spool(Rmaj, Rmin, Z)};
        #local Y2=Y-2*(Rmaj+Rmin);
        object {End}
        object {End translate <0, Y2, 0> }
        box {<-Rmaj, 0, -Eps>, <Rmaj, Y2, Z+Eps>}
        box {<-Rmaj-Rmin, 0, -Eps>, <-Rmaj+Eps, Y2, Rmin>} // left front
        box {<-Rmaj-Rmin, 0, Z-Rmin>, <-Rmaj+Eps, Y2, Z+Eps>} // left rear
        box {<Rmaj+Rmin, 0, -Eps>, <Rmaj-Eps, Y2, Rmin>} // right front
        box {<Rmaj+Rmin, 0, Z-Rmin>, <Rmaj-Eps, Y2, Z+Eps>} // right rear
      }
      object {Ycyl(<-Rmaj-Rmin, -Eps/2, Rmin>, Y2+Eps, Rmin)}
      object {Ycyl(<Rmaj+Rmin, -Eps/2, Rmin>, Y2+Eps, Rmin)}
      object {Ycyl(<-Rmaj-Rmin, -Eps/2, Z-Rmin>, Y2+Eps, Rmin)}
      object {Ycyl(<Rmaj+Rmin, -Eps/2, Z-Rmin>, Y2+Eps, Rmin)}
    }
  #end
#end

// Diameter == 1.0, lying on y=0, y=0.53 high.
#declare CoachBoltHead =
  union {
    intersection { // hemisphere, flattened, lifted a little
      sphere {0, 0.5}
      plane {-y, 0}
      scale <1, 0.5, 1>
      translate <0, 0.03, 0>
    }
    // the curved underside.
    intersection {
      torus {0.5 - 0.04, 0.04 translate <0, 0.03, 0>}
      plane{-y, 0}
    }
  };
    

#local Slot = object { LongSpool(0.9, 0.1, 2, 4) }
#local Hole = object { Spool(0.7, 0.3, 2) }

// To see the positives of what we subtract from the wall later
// object { Slot translate <-4, 1, -3> texture { floor_tex } }
// object { Hole translate <-1, 1, -3> texture { floor_tex } }

#local VertBolt = object {CoachBoltHead rotate -90*x }

#for (X, -3, 5)
object {CoachBoltHead translate <X, 0, -4 + Rand(1)> texture { floor_tex } }
#end

union{
  difference {
    box{<-30, 0, 0>, <30, 10, 1>}
    object { Hole translate <1, 2, 0>}
    object { Slot translate <4, 2, 0>}
  }
  object {VertBolt translate <1, 3 + Rand(1), 0>}
  object {VertBolt translate <-0.5, 3 + Rand(1), 0>}
  
  #local Rmin=0.1;
  isosurface {
    function {
      -z
       + SoftlyBinary(
      //f_crackle(x,y/10,0), 0,1, 0,0.2, 2
      f_noise3d(x,y/5,0) , 0,1, 0,0.1, 10
      )
      + max(0, sin(acos(min(abs(x), 1))))
      + 1-SoftlyBinary(abs(x-6)/2, 0, 1, 0, 1, 10)
      + Rmin * select(y-(1-Rmin), 0, 1-cos(asin( min(abs(y-(1-Rmin)) / Rmin, 1))))
      + Rmin * select(y-Rmin, 1-cos(asin( min(1-y/Rmin, 1))), 0)
      
    }
    contained_by { box {<-30, 0, -1> <30, 1, 2> }}
    max_gradient 120
    accuracy 0.00001
    translate <0, 10, -2>
  }
  box {<-30, 10, -Eps> <30, 11, 1> }
  object { Xcyl(<9.5, 5-Rmin, -1+Rmin>, 1, Rmin)}
  
  translate <0,-Eps,0.5>
  texture { floor_tex }
  rotate 10*y
}
    
sphere { <1, 0.5, 1.5>, 0.1 pigment {color rgb <1, 0, 0>}}
sphere { <4, 0.2, 1>, 0.1 pigment {color rgb <1, 0, 0>}}
