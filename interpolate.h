#ifndef INTERPOLATE_H
#define INTERPOLATE_H

double trilin_interp(double field000,double field001, double field010,
                     double field011, double field100, double field101,
                     double field110, double field111,
                     double xpos, double ypos, double zpos,
                     int x0, int y0, int z0, int x1, int y1, int z1);

double bilin_interp(double f00, double f10, double f01, double f11,
                        double x, double y);

double lin_interp(double f0, double f1, double x0,double x1, double x);

#endif
