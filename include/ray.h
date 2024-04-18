#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Define the ray class
class ray {

    // Define the public methods
    public:

        // Define the constructors
        ray() {}
        ray(const point3& origin, const Vector3& direction)
            : orig(origin), dir(direction)
        {}

        // Define the origin, direction, and at methods
        point3 origin() const  { return orig; }
        Vector3 direction() const { return dir; }
        point3 at(float t) const {
            return orig + t*dir;
        }

    public:
        // Define the public variables
        point3 orig;
        Vector3 dir;
};

#endif