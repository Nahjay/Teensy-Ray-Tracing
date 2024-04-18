#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

// Define the sphere class
class sphere : public hittable {
    public:

        // Define the public methods and constructors
        sphere(const point3& cen, float rad, shared_ptr<material> m) : center(cen), radius(fmax(0, rad)), mat_ptr(m) {};

        // Define the hit method
        virtual bool hit (const ray& r, interval ray_t, hit_record& rec) const override {
            // Define the variables
            Vector3 oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;
            auto discriminant = h*h - a*c;

            // Check if the ray intersects the sphere
            if (discriminant < 0) {
                return false;
            }

            // Calculate the root of the quadratic equation
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range
            auto root = (-h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (-h + sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            // Set the hit record
            rec.t = root;
            rec.p = r.at(rec.t);
            Vector3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            // Return true
            return true;
        }

    private:
        // Define the private variables
        point3 center;
        float radius;
        shared_ptr<material> mat_ptr;
};

#endif