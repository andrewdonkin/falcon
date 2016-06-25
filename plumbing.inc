#include "shapes3.inc"

#macro plumb_tube(zed, rad)
  cylinder {0, zed*z, rad transform moves_accum}
  #declare moves_accum = transform { translate zed*z moves_accum}
#end

#macro plumb_thinner(zed, radfrom, radto)
  cone {0, radfrom, zed*z, radto transform moves_accum}
  #declare moves_accum = transform { translate zed*z moves_accum}
#end

#macro plumb_twist(ang)
  #declare moves_accum = transform { rotate ang*z moves_accum}
#end  

#macro plumb_left(major, minor, ang)
  object {Segment_of_Torus (major, minor, -ang) translate <-major, 0, 0> transform moves_accum}
  #declare moves_accum = transform {
    translate <major, 0, 0> rotate <0, -ang, 0> translate <-major, 0, 0>
    moves_accum
  }
#end

#macro plumb_right(major, minor, ang)
  object { Segment_of_Torus (major, minor, -ang) scale <-1, 1, 1> translate <major, 0, 0> transform moves_accum}
  #declare moves_accum = transform {
    translate <-major, 0, 0> rotate <0, ang, 0> translate <major, 0, 0>
    moves_accum
  }
#end
