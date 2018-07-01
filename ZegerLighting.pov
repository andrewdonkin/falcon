camera {
  location <0,4,-3>/3
  look_at z/4
}
light_source {<-150,500,500> rgb <1.5,1.4,1.3>}

#declare Kleur=<1,1,1>;
#declare Vuilkleur=<.8,.075,.07>; // dirt colour
#declare Gewoon_vuil= // just dirty
texture {
  pigment {
    granite
    color_map {
      [0 rgb .2*Vuilkleur transmit .99]
      [1 rgb .1*Vuilkleur transmit .8]
    }
    rotate x*90
    translate -.5
    translate z*2
  }
  finish {ambient 0 diffuse .5}
}
#declare Gewoon_heel_vuil= // just very dirty
texture {
  pigment {
    granite
    color_map {
      [0 rgb .2*Vuilkleur transmit .99]
      [1 rgb .1*Vuilkleur transmit .6]
    }
    rotate x*90
    translate -.5
    //  scale <4*6,1,3*6>
    translate z*2
  }
  finish {ambient 0 diffuse .5}
}

#declare Scar=
  texture {
    pigment {
      crackle form <1,0,0>
      color_map {
        [0 rgbt <.1,.01,0,.8>]
        [.2 rgbt <.1,.01,0,1>]
      }
      scale .5
      turbulence .2
      lambda 3
      scale 4
    }
    normal {
      dents .1
    }
    finish {
      diffuse .5 ambient 0
    }
  }

plane {y,0
  #declare BS=1.0000;
  texture {
    pigment {
      average
      pigment_map {
        [1
          cells
          pigment_map {
            [0 
              cells
              color_map {
                [0 rgb .8*Kleur]
                [1 rgb 1*Kleur]
              }
              scale .5
            ]
            [.4 
              cells
              color_map {
                [0 rgb .8*Kleur]
                [1 rgb 1*Kleur]
              }
              translate <.25,.5,.2>
            ]
          }
          scale 1
        ]
        [1
          cells
          color_map {
            [0 rgb .8*Kleur]
            [1 rgb 1*Kleur]
          }
          translate <.25,1.5,.2>
          scale 2
          translate <2.25,1.5,3.5>
        ]
      }
      scale .25
    }
    normal {
      average
      normal_map {
        [1
          cells
          normal_map {
            [0 
              cells
              bump_size .25*BS
              scale .5
            ]
            [.4 
              cells
              bump_size .2*BS
              translate <.25,.5,.2>
            ]
          }
          scale 1
        ]
        [1
          cells
          normal_map {
            [0 
              cells
              bump_size .25*BS
              scale .5
            ]
            [.4 
              cells
              bump_size .2*BS
              translate <.25,.5,.2>
            ]
          }
          scale .25
        ]
        [1
          cells
          bump_size .125*BS
          translate <.25,1.5,.2>
          scale 2
          translate <2.25,1.5,3.5>
        ]
      }
      scale .125/8
    }
    finish {
      ambient 0 diffuse 1 brilliance 3 specular 1 roughness .01 
      metallic
    }
    scale 5
  }
  texture {Scar scale <.25,.25,1>}
  texture {Gewoon_vuil scale <.5,.25,1>} 
  texture {Gewoon_heel_vuil scale <.25,.125,.7>}
} 