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
  finish { phong 1 reflection 0.1 }
  normal { function {Crack(x,y,z)} 0.2}
};

plane { y, 0 texture { floor_tex } }

#if (1)
light_source {
  <0,0,0>             // light's position (translated below)
  color rgb 100.0       // light's color
  area_light
  <0.4, 0, 0> <0, 0.4, 0>
  18, 18
  adaptive 1          // 0,1,2,3...
  jitter              // adds random softening of light
  circular            // make the shape of the light circular
   orient              // orient light
  fade_power 2
  fade_distance 1
  //  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 diffuse 1 }}}
  translate <2, 5.5, -15>
}
#else

light_source { <2, 1.5, -15> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}
#end

camera { location <4, 7, -20> look_at <0, 0, 1> angle 30}
//camera { location <-0.1, 3, -4> look_at <2, 0, -0.9> angle 5}

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
        box {<-Rmaj-Rmin, 0, -Eps>, <-Rmaj, Y2, Rmin>} // left front
        box {<-Rmaj-Rmin, 0, Z-Rmin>, <-Rmaj, Y2, Z+Eps>} // left rear
        box {<Rmaj+Rmin, 0, -Eps>, <Rmaj, Y2, Rmin>} // right front
        box {<Rmaj+Rmin, 0, Z-Rmin>, <Rmaj, Y2, Z+Eps>} // right rear
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
object { Slot translate <-4, 1, -3> texture { floor_tex } }

#local Hole = object { Spool(0.7, 0.3, 2) }
object { Hole translate <-1, 1, -3> texture { floor_tex } }

#local Bolt = object {CoachBoltHead rotate -90*x }

#for (X, -3, 5)
object {CoachBoltHead translate <X, 0, -4 + Rand(1)> texture { floor_tex } }
#end

union{
    difference {
      box{<-10, -2, 0.5>, <10, 4, 2.5>}
      object { Hole translate <1, 1, 0.5>}
      object { Slot translate <4, 1, 0.5>}
    }
  object {Bolt translate <1, 3 + Rand(1), 0.5>}
  object {Bolt translate <-0.5, 3 + Rand(1), 0.5>}
  texture { floor_tex }
  rotate 10*y
}
    
sphere { <1, 0.5, 1.5>, 0.1 pigment {color rgb <1, 0, 0>}}
sphere { <4, 0.2, 1>, 0.1 pigment {color rgb <1, 0, 0>}}
