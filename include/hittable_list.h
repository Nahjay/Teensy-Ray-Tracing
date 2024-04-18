#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// Define the hittable_list class
class hittable_list : public hittable {
    // Define the public methods
    public:
        // Define the constructors
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        // Define the clear, add, and hit methods
        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        // Define the hit method
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            
            // Define the temporary hit record and the hit anything flag
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            // Loop through the objects in the list
            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            // Return the hit anything flag
            return hit_anything;
        }
    public:
        // Define the objects vector
        std::vector<shared_ptr<hittable>> objects;
};

#endif