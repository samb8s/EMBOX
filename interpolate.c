#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"grid.h"

double trilin_interp(double field000,double field001, double field010,
                     double field011, double field100, double field101,
                     double field110, double field111,
                     double xpos, double ypos, double zpos,
                     int x0, int y0, int z0, int x1, int y1, int z1){

    double xd, yd, zd;
    double c00, c10, c01, c11, c0, c1,c;

    xd = (xpos - x0);
    yd = (ypos - y0);
    zd = (zpos - z0);

    // start interpolating in x
    c00 = field000 * (1.0 - xd) +
                    field100 * xd;
    c10 = field010 * (1.0 - xd) +
                    field110 * xd;
    c01 = field001 * (1.0 - xd) +
                    field101 * xd;
    c11 = field011 * (1.0 - xd) +
                    field111 * xd;

    // interpolate in y
    c0 = c00 * (1.0 - yd) + c10 * yd;
    c1 = c01 * (1.0 - yd) + c11 * yd;

    // interpolate in z
    c = c0 * (1.0 - zd) + c1 * zd;

    return c;

}

double bilin_interp(double f00, double f10, double f01, double f11,
                        double x, double y){
    // x and y are expressed as fractions of the grid spacing
    // this means the problem is reduced to nearly a one-liner
    // (this works for a cubic grid)
    double f00term, f01term, f10term, f11term;

    f00term = f00 * (1-x)*(1-y);
    f10term = f10 * x * (1-y);
    f01term = f01 * (1-x) * y;
    f11term = f11 * x * y;

    // return the sum
    return f00term + f01term + f10term + f11term;
}

double lin_interp(double f0, double f1, double x0,double x1, double x){

    // f0 and f1 are field strength at x0 and x1
    // x is the position , x0<x<x1
    return f0 + (x - x0) * (f1-f0) / (x1 - x0);
}
