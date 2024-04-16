#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& cen, double rad) : center(cen), radius(fmax(0, rad)) {};

    private:
        point3 center;
        double radius;
};

#endif