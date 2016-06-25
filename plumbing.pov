
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
#declare moves_accum = transform {};

plumb_tube(2, 0.2)

plumb_left(0.9, 0.2, 20)

plumb_tube(3, 0.2)

plumb_right(3, 0.2, 20)
plumb_twist(20)

plumb_tube(1, 0.2)

plumb_right(0.3, 0.2, 45)

plumb_tube(2, 0.2)

plumb_right( 0.5, 0.2, 45)


plumb_thinner(4.1, 0.2, 0.1)
plumb_right ( 0.3, 0.1, 135 )
plumb_thinner(1.5, 0.1, 0.2)

plumb_right ( 0.5, 0.2, 35 )

plumb_tube(1.5, 0.2)

plumb_left ( 2.1, 0.2, 80 )

plumb_thinner(1.5, 0.2, 0.15)

plumb_thinner(1, 0.15, 0.1)

sphere {0, 0.1 transform moves_accum
        pigment { rgb y }
}

