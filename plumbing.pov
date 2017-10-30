#version 3.7;
#include "stdafx.inc"
#default {
  texture {
    pigment { rgb 1 }
    normal { bumps 0.07 scale 0.1}
    finish { ambient 0.1 specular 1 roughness 0.01 reflection 0.01}
  }
}


camera { location <0, 10, -15> look_at <1.4, 0, 5> angle 20}
//camera { location <0, 10, -15> look_at <2, 0, 1> angle 9}
//camera { location <8, 8, -15> look_at <1.5, 0, 0.7> angle 5} // spirals


global_settings {
   ambient_light 1
  assumed_gamma 1
}

light_source { <10, 50, -100> rgb 1 }

sky_sphere {
  //pigment {image_map {hdr "milkyway_Small.hdr" map_type 1 interpolate 2}}
//  pigment {image_map {hdr "donaucity_hd.hdr" map_type 1 interpolate 2}}
   pigment {image_map {hdr "galileo_probe.hdr" map_type 1 interpolate 2}}
  //rotate y*270 // glacier - puts the sun at our back
}

#include "plumbing.inc"
  
// Start.

plumb_start(0.2)
plumb_tube(2)
plumb_left(20, 0.9)
plumb_tube(0.5)
plumb_spiral_n(0, 0.8, 1, 0, 0.1)
plumb_tube(1.7)
plumb_right(20, 3)
plumb_twist(20)                     
plumb_tube(1)
plumb_right(45, 0.3)
plumb_tube(2)
plumb_right(45, 0.5)
plumb_thinner(0.1, 4.1)
plumb_right (135, 0.3 )
plumb_thinner(0.2, 1.5)
plumb_right ( 35, 0.5 )
plumb_tube(1.5)
plumb_left ( 80, 2.1 )
plumb_thinner(0.15, 1.5)
plumb_thinner(0.1, 1)
sphere {0, 0.1 transform plumb_transform()
        pigment { rgb y }
}

//-----------------------------

plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<0.6, 0, 0>})
plumb_transform_n(1, transform{translate<0.9, 0, 0>})

plumb_tube_n(0, 0.5)
plumb_tube_n(1, 0.5)
plumb_left_n(0, 45, 0) // Z increases by 0.1 with zero major radius param
plumb_tube_n(0, sqrt(2)/10) // Z increases by 0.1
plumb_right_n(0, 45, 0)
// Have moved forward three times the radius.
plumb_tube_n(0, 0.2)
// Have moved forward five times the radius.
plumb_right_n(0, 45, 0)
plumb_tube_n(0, sqrt(2)/10)
plumb_left_n (0, 45, 0) // total, 0.8

plumb_tube_n(1, 0.2)
plumb_radstep_n(1, 0.15, 0.01, "y")
plumb_splines_n(1, 0.1, 10, 1.1, 0)
plumb_tube_n(1, 0.1)
plumb_splines_n(1, 0.1, 10, 1.1, 9)
plumb_tube_n(1, 0.1)
plumb_radstep_n(1, 0.1, 0, "y")
plumb_tube_n(1, 0.4)

// If I got everything right, these ends should join seamlessly:
plumb_right_n(0, 90, 0.15)
plumb_left_n(1, 90, 0.15)
//camera { location <0, 10, -15> look_at <1, 0, 1> angle 3}

//-----------------------------
// Spirals

plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<1.3, 0, 0>})
plumb_transform_n(1, transform{translate<1.7, 0, 0>})
plumb_tube_n(0,0.2)
plumb_tube_n(1,0.2)

plumb_spiral_start_n(0, 0.3, 0.1)
plumb_spiral_n(0, 0.6, 2, 0.1, 0.1)
plumb_snowcone_finish_n(0, 0.6, 2, 0.1, 0.1)

plumb_wirehandle_n(1, 1.5, 2, 1, 2, 0.1, 0.1)

//-----------------------------

plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<2.2, 0, 0>})
plumb_tube_n(0,0.2)
plumb_ssweep_n(0, 2.2, 3, 0.2, 0.2)
//plumb_tube_n(0,0.2)


//union {
//plumb_start_n(1, 0.01)
//plumb_transform_n(0, transform{translate<2.2, 0, -0.1>})
//plumb_tube_n(0,3)
//pigment { rgb y }
//}


//-----------------------------

plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<2.8, 0, 0>})
plumb_transform_n(1, transform{translate<3.1, 0, 0>})

plumb_spiral_start_n(1, 0.2, 0.1)
plumb_spiral_n(1, 0.6, 1, 0.1, 0.0)
plumb_spiral_finish_n(1, 0.2, 0.1)

plumb_tube_n(0, 1.2)
plumb_splines_n(0, -0.22, 18, 1.5, 0)
plumb_tube_n(0, 0.3)
//plumb_tube_n(1, 0.5)
plumb_ribs_out_n(1, 0.5, 2, 0.3)

plumb_tube_n(0, 0.5)
plumb_tube_n(1, 0.1)
plumb_ribs_in_n(1, 0.4, 8, 0.06)

plumb_right_n(0, 180, 1.3)
plumb_right_n(1, 180, 1)
plumb_twist_n(0, 20)
plumb_twist_n(1, 20)
plumb_right_n(0, 130, 1.3)
plumb_right_n(1, 130, 1)

plumb_tube_n(0, 1)
plumb_tube_n(1, 1)

plumb_radstep_n(0, 0.2, 0, "n")
//plumb_tube_n(0, 0.1)

plumb_left_n(0, 0.5, 90)

plumb_radstep_n(0, 0.1, 0, "n")
plumb_tube_n(0,0.22)

//-----------------------------

union {
  object {plumb_marshmallow}
  Zcyl(<0, 1.5, 0>, -10, 1)
  scale 0.1
  rotate 45*y
  translate <1.4, 0, 5>
}


#if (0)
parametric{
  function{ cos(u)+0.4*cos(v/8*2*pi)} // x(u,v)
  function{ sin(u)+0.4*sin(v/8*2*pi)}   // y(u,v)
  function{ v }        // z(u,v)
  <0,0>, <2*pi,8>  // start, end(u,v)
  contained_by {box {<-1.5,-1.5,0>,<1.5,1.5,8>}}
  max_gradient 7
  accuracy 0.005
  precompute 10 x,y,z
  scale 0.1
  translate <1.9, 0, -0.8>
}

#end
