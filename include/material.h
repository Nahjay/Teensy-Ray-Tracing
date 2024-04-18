#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracing.h"

class hit_record;

// Define the material class
class material {

    // Define the public methods
    public:

        // Define the scatter method
        virtual ~material() = default;
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const = 0;
};

// Define the lambertian class (Better reflectance of light)
class lambertian : public material {
    public:

        // Define the lambertian constructor
        lambertian(const Color& a) : albedo(a) {}

        // Define the scatter method
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const override {
            Vector3 scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            // Set the scattered ray
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        // Define the albedo color
        Color albedo;
};

// Define the metal class (EVEN Better reflectance of light!!!)
class metal : public material {
    // Define the public methods
    public:

        // Define the metal constructor
        metal(const Color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        // Define the scatter method
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const override {
            Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        // Define the albedo color and fuzz
        Color albedo;
        float fuzz;
};

class dielectric : public material {

    // Define the public methods
    public:

        // Define the dielectric constructor
        dielectric(float index_of_refraction) : ir(index_of_refraction) {}

        // Define the scatter method
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const override {
            // Set the attenuation color
            attenuation = Color(1.0, 1.0, 1.0);
            float refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            // Calculate the direction of the ray
            Vector3 unit_direction = unit_vector(r_in.direction());
            float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            // Check if the ray can refract
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3 direction;

            // Calculate the direction of the ray
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            // Set the scattered ray
            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        // Define the index of refraction
        float ir;

    private:
        // Define the reflectance method
        static float reflectance(float cosine, float ref_idx) {
            // Use Schlick's approximation for reflectance
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine), 5);
        }
};
#endif