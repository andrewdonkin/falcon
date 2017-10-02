#version 3.7;
#macro ycyl(O, Height, R) cylinder{O, O+y*Height, R} #end
#macro xcyl(O, Width, R) cylinder{O, O+x*Width, R} #end
#macro zcyl(O, Len, R) cylinder{O, O+z*Len, R} #end
