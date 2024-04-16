#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

class material;
class hit_record {
    public:
        point3 p;
        Vector3 normal;
        double t;
        bool front_face;
        std::shared_ptr<material> mat_ptr;

        inline void set_face_normal(const ray& r, const Vector3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }

};
class hittable {
    public:
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif