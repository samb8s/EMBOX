#ifndef INTERPOLATE_H
#define INTERPOLATE_H

double trilin_interp();

double bilin_interp(double f00, double f10, double f01, double f11,
                        double x, double y);

double lin_interp(double f0, double f1, double x0,double x1, double x);

#endif
