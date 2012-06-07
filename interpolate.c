#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

double trilin_interp(){
}

double bilin_interp(double f00, double f10, double f01, double f11,
                        double x, double y){
    // x and y are expressed as fractions of the grid spacing
    // this means the problem is reduced to nearly a one-liner
    //
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
