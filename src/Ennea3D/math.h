#ifndef MORE_MATH_H
#define MORE_MATH_H

double EnneaDistance(double camx, double camy, double camz, double x, double y, double z);

double EnneaDistance(double camx, double camy, double camz, double x, double y, double z)
{
        return sqrt(((camx - x) * (camx - x)) + ((camy - y) * (camy - y)) + ((camz - z) * (camz - z)));
}

#endif