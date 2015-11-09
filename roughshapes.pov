#include "roughshapes.inc"
//#include "colors.inc"

global_settings {
   ambient_light 0
  ambient_light 0 radiosity {  }
   max_trace_level 200
  assumed_gamma 1
}
#default {
  texture {
    pigment { rgb 1 }
//    normal { bumps 1 }
    finish { ambient 0.1 }
  }
}

light_source { <47, 9, -50> rgb 1 }
plane {y, 0
  pigment {
    checker pigment{rgb 1} pigment {rgb 0.8}
  }
}

object {roughbox (<1, 1, 1>, 0.05, 10, 3) rotate 0*y}
object {roughcylinder ( <1.5, 0, 0.5>, <1.6, 1, 0.3>, 0.5, 30, 6)}
object {gappywall (<5, 0.5, 0.1>, 5, 0.2, 0.4) translate -0.3*z}
object {roughgappywall (<5, 0.4, 0.1>, 0.01, 5, 0.1, 0.2) translate -1.3*z}

camera { location <0, 4, -3> look_at <0, 0, 0> }

camera { location <2, 2, -2> look_at <1, 1, 0> }  // cylinder
camera { location <0, 3, -2> look_at <0, 1, 0> }  // box
camera { location <-1, 1, -1.5> look_at <1, 0.5, -1.5> }  // wall
