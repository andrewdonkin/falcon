// requires hexdron.inc
#declare rseed = seed(1);

#default {
  texture {
    pigment { rgb 1 }
//    normal { bumps 1 }
    finish { ambient 0.1 }
  }
}

// Andre's plans are 50 pixels per unit (inch).

// 20 degree cutout angles beside docking rings meet z = 0 at x = +/- 10.

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
  // Front and rear faces at |x|=20 are 35deg from vertical and would meet y=0 at |z|=5.1

  #local dockhalf =   hexdron(
    <5.907, 5.1, -1.938>,
    <5.907, 5.1,  1.938>,
    <   20, 4.5,  1.938>,
    <   20, 4.5, -1.938>,
    <  3.3, 0.6, -5.0  >, // set y=0.6 so sides are planes:  otherwise two triangles.
    <  3.3, 0.6,  5.0  >,
    <   20,   0,  5.0  >,
    <   20,   0, -5.0  >,
    0.1)

  object {dockhalf}
  object {dockhalf rotate 180*x}
}

#declare mandible = union {
 // Inner mandible angles are c.2.5 degrees off z axis:
 // In-body end is x=+/-5, z=0.  Front-most extent is x=+/-3.313, z=-39.375.
 // mandible reaches dia 45 dome at x=4, z=-22

  // main body, solid
  hexdron(
    <3.313,  1, -39.375>,
    <    4,  1, -22>,
    <   20,  1, -10.1>,
    < 5.75,  1, -39.375>,
    <3.313, -1, -39.375>,
    <    4, -1, -22>,
    <   20, -1, -10.1>,
    < 5.75, -1, -39.375>,
    0.02)

  // Ferrari V12 engine cover, facing -z, x=0 to x=2.4, y=+/-1

  // supporting macro.  call it a spark plug poking out to z=-0.1
  #macro V12plug(randseed)
    union {
      union {
        object {
//          roughcylinder(-0.1*z, 0.1*z, 0.15)
          roughcylinder(0.1*z, -0.1*z, 0.15, 15, 5)
          scale <1,1.5,1>
        }
        sphere {<0, 0.18, -0.1>, 0.03}
        sphere {<0, -0.18, -0.1>, 0.03}
        rotate (-20-(rand(randseed)*30))*z
      }
      difference {
        roughcylinder(<0, 0, 0>, <0, 0, -0.2>, 0.13, 15, 5)
        cylinder{<0, 0, -0.3>, <0, 0, -0.1>, 0.10}
      }
    }
  #end // V12cyl macro

  difference {
    // a rectangular section in the centre minus a hole for a headlight
    union {
//      box {<0, -0.35, -0.3>, <2.4, 0.35, 0>} // between the two banks of cylinders
      object {roughbox (<2.4, 0.3, 0.7>, 0.03, 10, 10) rotate -90*x translate <0, -0.35, 0>} // between the two banks of cylinders
      object {roughbox (<2.2, 0.4, 0.6>, 0.02, 10, 20) rotate -90*x translate <0.1, -0.3, 0>} // smaller, poking further toward -Z
      #for (I, -0.2, 0.2, .03)   // vanes
        box{<0.15+rand(rseed)/40, I-0.01, -0.45>, <2.25-rand(rseed)/40, I+0.01, 0>}
      #end // for, vanes
      box {<0.8, -0.05, -.45>, <0.9, 0.05, 0>} // two flat patches in the vanes
      box {<1.5, -0.05, -.45>, <1.6, 0.05, 0>}
      // trim between cam cover and spark plugs
      cylinder {<0.1, 0.4, -0.04>, <2.3, 0.4, -0.04> 0.03}
      cylinder {<0.1, -0.4, -0.04>, <2.3, -0.4, -0.04> 0.03}
      // cylinders, two banks of six
      #for (I, 0.3, 2.1, 0.36)
        object {V12plug(rseed) rotate 10*x translate <I, 0.7 + rand(rseed)/40, 0>}
        object {V12plug(rseed) rotate -10*x translate <I, -0.7 - rand(rseed)/40, 0>}
        cylinder {<I, -0.6, -0.05>, <I, 0.6, -0.05>, 0.03}
      #end // for
    }
    // hole for headlight
    cylinder {<0.6, 0, -1>, <0.6, 0, 0>, 0.15}
    
    translate <3.313, 0, -39.375>
  }

}


#declare body = union {
#if (1)
  difference {
    union {
//      difference {
	      intersection { // 45 dia shells
	        sphere { <0, -72.64, 0>, 77.48 } // top shell
	        sphere { <0,  72.64, 0>, 77.48 } // bottom
	        cylinder { <0, -5, 0>, <0, 5, 0>, 22.5 }
	      }
	      // TODO subtract service ports here
//      }
    
      intersection { // 48 dia shells
	      union {
      	  difference {
      	    sphere { <0, -72.64, 0>, 77.48 }
      	    sphere { <0, -72.64, 0>, 77.38 }
      	  }
      	  difference {
      	    sphere { <0, 72.64, 0>, 77.48 }
      	    sphere { <0, 72.64, 0>, 77.38 }
      	  }
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

