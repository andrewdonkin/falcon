// requires hexdron.inc
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

#declare docking = union {

  // docking passage top and bot tilt 2.5 degrees from horizontal,
  // approaching each other as |x| increases.  If extended inboard,
  // the top would reach x=0 at the same y as the top of the ring
  // there, y=5.375.
  //
  // Inner near-vertical face is 25deg from vertical.  Starts at
  // x=5.907, y=5.1.  If extended down it would reach y=0 at x=3.3.
  //
  // Outer vertical face (to which docking ring is attached) is at
  // x=20, y=4.5.
  //
  // Front and rear faces are 35deg from vertical and would meet y=0 at |z|=5.1

  #local dockhalf =   hexdron(
    <5.907, 5.1, -1.938>,
    <5.907, 5.1,  1.938>,
    <   20, 4.5,  1.938>,
    <   20, 4.5, -1.938>,
    <  3.3, 0.6, -5.0  >,
    <  3.3, 0.6,  5.0  >,
    <   20,   0,  5.0  >,
    <   20,   0, -5.0  >,
    0.1)

  object {dockhalf}
  object {dockhalf rotate 180*x}
}

#declare mandible = 
 // mandible reaches dia 45 dome at x=4, z=-22
  hexdron(
    <3.313,  1, -39.375>,
    <    4,  1, -22>,
    <   20,  1, -10.1>,
    < 5.76,  1, -39.375>,
    <3.313, -1, -39.375>,
    <    4, -1, -22>,
    <   20, -1, -10.1>,
    < 5.76, -1, -39.375>,
    0.1)



#declare body = union {
#if (1)
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
#end
  object { docking }
  object {docking rotate 180*y}

  object {mandible}
  object {mandible rotate 180*z}

}

