#include "roughshapes.inc"

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
plane {y, 0}

object {roughbox (<1, 1, 1>, 0.05, 10, 3) rotate 0*y}
object {roughcylinder ( <1.5, 0, 0.5>, <1.6, 1, 0.3>, 0.5, 60)}

camera { location <0, 4, -3> look_at <0, 0, 0> }

//camera { location <2, 3, -2> look_at <1, 1, 0> }  // cylinder
camera { location <0, 3, -2> look_at <0, 1, 0> }  // box
