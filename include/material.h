#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracing.h"

class hit_record;

class material {
    public:
        virtual ~material() = default;
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const Color& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const override {
            Vector3 scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        Color albedo;
};

#endif