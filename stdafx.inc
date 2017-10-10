#ifndef(stdafx_inc)
// The name was a joke; now I am regretting it
#declare stdafx_inc = 1;

#version 3.7;

#declare Eps=0.000001;
#macro ycyl(O, Height, R) cylinder{O, O+y*Height, R} #end
#macro xcyl(O, Width, R) cylinder{O, O+x*Width, R} #end
#macro zcyl(O, Len, R) cylinder{O, O+z*Len, R} #end

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
// Returns 1 when X=0, <1 as abs(X) increases, reaching 0 at X=1.
#declare Softly = function(X, GS, GE, TS, TE) {
  (TS+(TE-TS) * ((1 - cos( min((X-GS)/(GE-GS), 1) * pi)) /2 ) )
  //(TS+(TE-TS) * min((X-GS)/(GE-GS), 1))
}

// Returns 1 when X=0, <1 as abs(X) increases.  Reaches 0 when X=ZeroAtHere.
//  #declare OneAtZero = function(X, Power) {1 / (Power * (abs(X)+ 1/Power)) }
#declare OneAtZero = function(X, ZeroAtHere) {Softly(abs(X)/ZeroAtHere, 0, 1, 1, 0)}

#end
