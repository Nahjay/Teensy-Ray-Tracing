#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"


class material;

// Define the hit record class
class hit_record {

    // Define the public methods
    public:

        // Define the constructors
        point3 p;
        Vector3 normal;
        float t;
        bool front_face;
        std::shared_ptr<material> mat_ptr;


        // Define the set_face_normal method
        inline void set_face_normal(const ray& r, const Vector3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }

};

// Define the hittable class
class hittable {

    // Define the public methods
    public:

        // Define the hit method
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif