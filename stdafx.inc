#version 3.7;
#macro ycyl(O, Height, R) cylinder{O, O+y*Height, R} #end
#macro xcyl(O, Width, R) cylinder{O, O+x*Width, R} #end
#macro zcyl(O, Len, R) cylinder{O, O+z*Len, R} #end

// Thanks to Vince, Geometry for Computer Graphics.
// page 60
#macro Matrix2determinated(a,b,c,d)
 (a * d - b*c)
#end
#macro Matrix3determinated(a,b,c,d,e,f,g,h,i)
 (a * (e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g))
#end

// Let's hope they don't make a Matrix 4.
// page 59
#macro PlaneFrom3Points(O, V1, V2)
 #local A = Matrix3determinated(1, O.y, C.z,  1, V2.y, V2.z,  1, V3.y, V3.z);
 #local B = Matrix3determinated(O.x, 1, O.z,  V2.x, 1, V2.z,  V3.x, 1, V3.z);
 #local C = Matrix3determinated(O.x, O.y, 1,  V2.x, V2.y, 1,  V3.x, V3.y, 1);
 (A, B, C, -1 * sqrt(A * O.x + B * O.y + C * O.z))
#end