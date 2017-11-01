//#include ".inc"
#local Eps=0.00001;
global_settings {
  ambient_light 0 // radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

#local floor_tex = texture {
    pigment {checker color rgb <0.3, 0.3, 1>, colour rgb <0.8,0.8,1>}
    finish { phong 1 reflection 0.1 }
};

plane { y, -2
  texture {
    floor_tex
  }
}

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

camera { location <4, 7, -15> look_at <0, 0, 1> angle 50}
//camera { location <-0.1, 3, -4> look_at <2, 0, -0.9> angle 5}

#macro Spool( Rmaj, Rmin, Len )
  difference {
    union {
      cylinder { <0,0,-Eps>, <0,0, Len+Eps>, Rmaj }
      cylinder { <0,0,-Eps>, <0,0, Rmin>, Rmaj+Rmin }
      cylinder { <0,0,Len-Rmin>, <0,0, Len+Eps>, Rmaj+Rmin }
    }
    torus {Rmaj+Rmin, Rmin rotate 90*x translate <0,0,Rmin> }
    torus {Rmaj+Rmin, Rmin rotate 90*x translate <0,0,Len-Rmin> }
  }
#end

object {
  Spool(1, 0.3, 2)
  translate <-2, 0, -2>
  texture { floor_tex }
}

difference {
  box{<-10, -2, 0.5>, <10, 4, 2.5>}
  object {Spool(1, 0.3, 2) translate <1, 0, 0.5>}
  texture { floor_tex }
  rotate 10*y
}

