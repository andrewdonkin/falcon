#macro hexdron(
  tlf, tlr, trr, trf, // top quadrilateral
  blf, blr, brr, brf, // bottom quadrilateral
  smallr	      // radius of rounded edges
  )
  
  #if (0)   // assume our shape is uninteresting enough that we can do this:
    #local tlf = tlf + < smallr, -smallr, smallr>;
    #local tlr = tlr + < smallr, -smallr,-smallr>;
    #local trr = trr + <-smallr, -smallr,-smallr>;
    #local trf = trf + <-smallr, -smallr, smallr>;
    #local blf = blf + < smallr,  smallr, smallr>;
    #local blr = blr + < smallr,  smallr,-smallr>;
    #local brr = brr + <-smallr,  smallr,-smallr>;
    #local brf = brf + <-smallr,  smallr, smallr>;
  #end
  
  #if (vlength(vnormalize(vcross(tlr-tlf, trf-tlf))
    - vnormalize(vcross(trf-trr, tlr-trr)) ))
    #warning "not a flat top quadrilateral"
  #end
  #if (vlength(vnormalize(vcross(blr-blf, brf-blf))
    - vnormalize(vcross(brf-brr, blr-brr)) ))
    #warning "not a flat bottom quadrilateral"
  #end
  // should do the other four too
  
  // Draw a quadrilateral between four points.  If not on a plane, it will miss one of the four.
  #macro hexface (
    lf, lr, rr, rf    // rectangle
    )
    // use smallr from parent scope, ewww
    #local nml = smallr * vnormalize(vcross(lr-lf, rf-lf));
    
    triangle {lf+nml, lr+nml, rr+nml}
    triangle {rr+nml, rf+nml, lf+nml}
  #end

  union {
    hexface(tlf, tlr, trr, trf) // top
    hexface(blf, brf, brr, blr) // bottom, clockwise
    hexface(blf, tlf, trf, brf) // front
    hexface(blr, tlr, tlf, blf) // left
    hexface(brr, trr, tlr, blr) // rear, still going clockwise from apparant bot left
    hexface(brf, trf, trr, brr) // right

    #if (1)
      sphere {tlf, smallr}  sphere {tlr, smallr}  sphere {trr, smallr}  sphere {trf, smallr}
      sphere {blf, smallr}  sphere {blr, smallr}  sphere {brr, smallr}  sphere {brf, smallr}
      cylinder {tlf, tlr, smallr}  cylinder {tlr, trr, smallr} // around the top
      cylinder {trr, trf, smallr}  cylinder {trf, tlf, smallr}
      cylinder {blf, blr, smallr}  cylinder {blr, brr, smallr} // around the bot
      cylinder {brr, brf, smallr}  cylinder {brf, blf, smallr}
      cylinder {tlf, blf, smallr}  cylinder {tlr, blr, smallr} // left verticals
      cylinder {trf, brf, smallr}  cylinder {trr, brr, smallr} // right verticals
    #end
  }
    
#end
  
