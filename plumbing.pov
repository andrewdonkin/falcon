#version 3.7;
#default {
  texture {
    pigment { rgb 1 }
    normal { bumps 0.07 scale 0.1}
    finish { ambient 0.1 }
  }
}


camera { location <0, 10, -15> look_at <1.4, 0, 5> angle 20}
camera { location <0, 10, -15> look_at <1.6, 0, 0> angle 8}


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

plumb_tube(3)

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
plumb_transform_n(0, transform{translate<1, 0, 0>})
plumb_transform_n(1, transform{translate<1.3, 0, 0>})

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

plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<1.6, 0, 0>})
plumb_transform_n(1, transform{translate<1.9, 0, 0>})

plumb_tube_n(0, 0.2)
plumb_splines_n(0, -0.22, 18, 1.5, 0)
plumb_tube_n(0, 0.3)
//plumb_tube_n(1, 0.5)
plumb_ribs_out_n(1, 0.5, 4, 0.05)

plumb_tube_n(0, 0.5)
plumb_tube_n(1, 0.1)
plumb_ribs_in_n(1, 0.4, 8, 0.03)

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

#if (0)  
#undef F
#declare  F = function {y+cos(z*2*pi)*0.02-1-0.02}
isosurface {
  function { F(x, sqrt(y*y+x*x), z*2) }
  contained_by {box {<-1.5,-1.1,0>,<1.5,1.1,4>}}
  scale 0.1
  translate <1.9, 0, -0.4>
}
#undef F
#declare  F = function {y-cos(z*2*pi)*0.02-1+0.02}
isosurface {
  function { F(x, sqrt(y*y+x*x), z*2) }
  contained_by {box {<-1.1,-1.1,0>,<1.1,1.1,4>}}
  scale 0.1
  translate <1.9, 0, -0.9>
}

parametric{
  function{ cos(u)+0.1*sin(v*pi)} // x(u,v)
  function{ sin(u)+0.1*sin(v*pi)}   // y(u,v)
  function{ v }        // z(u,v)
  <0,0>,<2*pi,4>  // start, end(u,v)
  contained_by {box {<-1.5,-1.5,0>,<1.5,1.5,4>}}
  max_gradient 4
  accuracy 0.003
  precompute 9 x,y,z
  scale 0.1
  translate <1.9, 0, -0.4>
}
 #end

plumb_start_n(1,0.1)
plumb_transform_n(0, transform{translate<1.9, 0, -0.5>})
plumb_tube_n(0, 0.1)


