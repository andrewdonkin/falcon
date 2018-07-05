#ifndef(Stdafx_inc)
#version 3.7;
// The name was a bad joke.
#declare Stdafx_inc = 1;


#declare Eps=0.000001;

#declare RandSeed=seed(1);

// can take a vector argument
#macro Rand(PlusOrMinus) ((rand(RandSeed) - 0.5) * PlusOrMinus * 2) #end
  
#macro Ycyl(O, Height, R) cylinder{O, O+y*Height, R} #end
#macro Xcyl(O, Width, R) cylinder{O, O+x*Width, R} #end
#macro Zcyl(O, Len, R) cylinder{O, O+z*Len, R} #end

// Thanks to Vince, Geometry for Computer Graphics, page 60.
// Returns a normalised vector, since that equation expects one.
#macro Matrix2determinated(a,b,c,d)
 (a * d - b*c)
#end
#macro Matrix3determinated(a,b,c,d,e,f,g,h,i)
 (a * (e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g))
#end
// Same, but takes three three-vectors instead of nine floats.
#macro Matrix3determinatedV(V1, V2, V3)
 Matrix3determinated(V1.x, V1.y, V1.z,  V2.x, V2.y, V2.z,  V3.x, V3.y, V3.z)
#end
// Let's hope they don't make a Matrix 4.

// Plane distance from origin along a vector, given that vector and one point.
// Vince p.60 1.15.7, except this D is negated.
#macro UNUSEDPlaneDFromNormalPoint(N, P)
  #if (vlength(N) = 0)
  # local D = 0;
  #else
  # local D = (N.x * P.x + N.y * P.y + N.z * P.z) / vlength(N);
  #end
  (D)
#end
  
// Plane vector and distance from origin along that vector, from three points.
// Shared vertex first, then go clockwise
// Vince p.59, except this D is negated.
#macro PlaneFrom3Points(O, V1, V2)
 #local A = Matrix3determinated(1, O.y, O.z,  1, V1.y, V1.z,  1, V2.y, V2.z);
 #local B = Matrix3determinated(O.x, 1, O.z,  V1.x, 1, V1.z,  V2.x, 1, V2.z);
 #local C = Matrix3determinated(O.x, O.y, 1,  V1.x, V1.y, 1,  V2.x, V2.y, 1);
 #if (A*A + B*B + C*C = 0)
 # local D = 0;
 #else
 # local D = (A * O.x + B * O.y + C * O.z) / sqrt(A*A + B*B + C*C);
 #end
// #warning concat( "PF3P ", str(A, 1, 2), " ", str(B, 1, 2), " ", str(C, 1, 2), " ", str(D, 1, 2))
 (vnormalize(<A, B, C>), D)
#end

// Interpolate author: Margus Ramst
// X ranges between GS and GE -> result ranges between TS and TE.
#declare Softly = function(X, GS, GE, TS, TE) {
  (TS+(TE-TS) * ((1 - cos( min((X-GS)/(GE-GS), 1) * pi)) /2 ) )
  //(TS+(TE-TS) * min((X-GS)/(GE-GS), 1)) // linear
}
// Same as SoftlyP with more arguments.
#declare SoftlyBinary = function(X, GS, GE, TS, TE, Power) {
  select(X-GS, TS, TS,
    select(GE-X, TE, TE,
      select(X-(GE-GS)/2,
        pow(2*(X-GS)/(GE-GS), Power),           // in < 0.5
        2-pow(2*(1-(X-GS)/(GE-GS)), Power)      // in >= 0.5
      ) / 2 * (TE-TS) + TS
    )
  )
}
// (input,output) always goes through (0,0), (0.5,0.5), and (1,1),
// but bends either side (unless Power==1 -> linear).
// Very steep at Power=4.
// If Power < 1, has the opposite of the desired effect:  starts and
// ends like a rocket with a long flat bit in the middle.
// Like a Michael Chrichton book.
// If Power == 2, practically identical to Interpolate(0) and softly().
#declare SoftlyP = function(in, Power) {
  select(in, 0, 0,
    select(1-in, 1, 1,
      select(in-0.5,
        pow(2*in, Power),          // in < 0.5
        1,
        2-pow(2*(1-in), Power)     // in > 0.5
      )/2
    )
  )
}

// (in, out) always goes through (0,0), (Attack,1), (0.5,1) (Decay,1) (1,0)
// A bell curve with straight sides.  Used as a multiplier on randomness.
// 
#declare SharpBell = function(in, Attack, Decay) {
//  select(in, 0,
//    select(1-in, 0,  Makes it slow.  Add another 0 parameter, worse.
      select(in-Attack,
        SoftlyP(in/Attack, 2),
        1,
        select(in-Decay, 1, 1, 1-SoftlyP(in-Decay,2)/(1-Decay))
      )
//    )
//  )
}



// Returns 1 when X=0, (1,0] as abs(X) increases.  Reaches 0 when abs(X)=abs(ZeroAtHere).
//  #declare OneAtZero = function(X, Power) {1 / (Power * (abs(X)+ 1/Power)) }
#declare OneAtZero = function(X, ZeroAtHere) {Softly(abs(X)/abs(ZeroAtHere), 0, 1, 1, 0)}
// Returns 0 when X=0, (0,1] as abs(X) increases.  Reaches 1 when abs(X)=abs(OneAtHere).
#declare ZeroAtZero = function(X, OneAtHere) {Softly(abs(X)/(OneAtHere), 0, 1, 0, 1)}

#end
