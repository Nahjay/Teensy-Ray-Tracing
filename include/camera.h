#ifndef CAMERA_H
#define CAMERA_H

#include "ray_tracing.h"
#include "hittable.h"

class camera {
    public:

    private:

    // Define the ray color function
    Color ray_color(const ray& r, const hittable& world) {
    hit_record rec;

    if (world.hit(r, interval(0, inf), rec)) {
        return 0.5 * Color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    }

    Vector3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
}


#endif