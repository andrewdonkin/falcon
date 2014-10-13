#default {
  texture {
    pigment { rgb 1 }
//    normal { bumps 1 }
    finish { ambient 0.1 }
  }
}

// Andre's plans are 50 pixels per unit (inch).

// 20 degree angles beside docking rings meet z = 0 at x = +/- 10
// inner mandible angles are c.2.5 degrees off x axis.
// One end is x=+/-5, z=0.  Other is x=+/-3.313, z=-39.375.

// docking passages tilt 2.5 degrees from horizontal, handily starting at x=0,z=0.
#declare docking =   intersection { // docking passage
  box { <0, -5, -3.625>, <20, 5, 3.625> }
  plane { -y 0 rotate 2.5*z translate -5.375*y } // bottom
  plane { y, 0 rotate -2.5*z translate 5.375*y } //top
  plane { -x, 0 rotate -25*z translate 3.3*x } // inner edge roof
  plane { -x, 0 rotate  25*z translate -3.3*x } // inner edge floor
  plane { -z, 0 rotate  35*x translate -5.2*z } // leading top wall, sloped
  plane {  z, 0 rotate -35*x translate  5.2*z } // trailing top wall, sloped
  plane { -z, 0 rotate -35*x translate -5.2*z } // leading under wall, sloped
  plane {  z, 0 rotate  35*x translate  5.2*z } // trailing under wall, sloped
  
}


#declare body = union {
  difference {
  union {
    difference {
      intersection { // 45 dia shells
	sphere { <0, -72.64, 0>, 77.48 } // top shell
	sphere { <0,  72.64, 0>, 77.48 } // bottom
	cylinder { <0, -5, 0>, <0, 5, 0>, 22.5 }
      }
    }
    
    intersection { // 48 dia shells
      union {
	difference {
	  sphere { <0, -72.64, 0>, 77.48 }
	  sphere { <0, -72.74, 0>, 77.48 } }
	difference {
	  sphere { <0, 72.64, 0>, 77.48 }
	  sphere { <0, 72.74, 0>, 77.48 } }
      }
      cylinder { <0, -5, 0>, <0, 5, 0>, 24} // larger radius
      // larger radius stops 42.3 degrees from the X axis
      union {
	box { <-25, -5, 0>, <25, 5, 25> rotate 42.3 * y}
	box { <-25, -5, 0>, <25, 5, 25> rotate -42.3 * y}
      }
      //mandibles tangent 45u circle at 29.5 degrees from X axis.
    }
  }
  intersection { // left (starboard) docking wedge cut
    plane { -z, 0 rotate -20*y translate -10*x}
    plane {  z, 0 rotate  20*y translate -10*x}
  }
  intersection { // right (port) docking wedge cut
    plane { -z, 0 rotate  20*y translate 10*x}
    plane {  z, 0 rotate -20*y translate 10*x}
  }
  }
  object { docking }
  object {docking rotate 180*y}
}

