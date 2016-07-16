
#default {
  texture {
    pigment { rgb 1 }
    normal { bumps 0.07 scale 0.1}
    finish { ambient 0.1 }
  }
}


camera { location <0, 10, -15> look_at <0, 0, 5> angle 30}


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

plumb_left(0.9, 20)

plumb_tube(3)

plumb_right(3, 20)
plumb_twist(20)


plumb_tube(1)

plumb_right(0.3, 45)

plumb_tube(2)

plumb_right( 0.5, 45)


plumb_thinner(4.1, 0.1)
plumb_right ( 0.3, 135 )
plumb_thinner(1.5, 0.2)

plumb_right ( 0.5, 35 )

plumb_tube(1.5)

plumb_left ( 2.1, 80 )

plumb_thinner(1.5, 0.15)

plumb_thinner(1, 0.1)

sphere {0, 0.1 transform plumb_transform()
        pigment { rgb y }
}


plumb_start_n(2, 0.1)
plumb_transform_n(0, transform{translate<1, 0, 0>})
plumb_transform_n(1, transform{translate<1.3, 0, 0>})


plumb_tube_n(0, 2)
plumb_tube_n(1, 2)
plumb_right_n(0, 1.3, 180)
plumb_right_n(1, 1, 180)
plumb_twist_n(0, 20)
plumb_twist_n(1, 20)
plumb_right_n(0, 1.3, 180)
plumb_right_n(1, 1, 180)

plumb_tube_n(0, 1)
plumb_tube_n(1, 1)

plumb_radstep_n(0, 0.2, 0)
plumb_radstep_n(0, 0.3, 0)
plumb_radstep_n(0, 0.4, 0)
