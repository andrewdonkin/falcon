global_settings {
 //  ambient_light 0
  //ambient_light 0 // radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
}

plane { y, 0
  pigment {
    checker color rgb <0,0,0>, colour rgb <1,1,1>
  }
}

difference {
  cylinder { <0,0,0>,<0,1,0>, 5
    texture { pigment { color rgb<1,0,1> }
      // finish  { phong 0.5 reflection{ 0.00 metallic 0.00} } 
    } // end of texture
  }
  plane {-x, 0 
    texture { pigment { color rgb<1,0.5,0> }
      // finish  { phong 0.5 reflection{ 0.00 metallic 0.00} } 
    } // end of texture

  }
  cylinder { -0.1*y, 1.1*y, 2
    translate (5 - 2)*z rotate -y * degrees(asin (2/(5-2)))
    // translate <-2, 0, 2 / tan(asin (2 / (5 - 2)))> // same thing
    pigment { color rgb<1,0.5,0> }
      // finish  { phong 0.5 reflection{ 0.00 metallic 0.00} } 
  }
  // cutaway_textures
}


light_source { <4.7, 3, -4> rgb 1
  looks_like {sphere {0, 0.1 texture {pigment {color rgb <1,1,1>}} finish { ambient 1 }}}
}


camera { location <2, 10, -10> look_at <0, 0, 0> }
