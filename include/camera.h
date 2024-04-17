#ifndef CAMERA_H
#define CAMERA_H

#include "ray_tracing.h"
#include "hittable.h"
#include "material.h"
#include <Adafruit_ILI9341.h>
#include <limits>

const double infi = std::numeric_limits<double>::infinity();

class camera {
    public:
        // Camera constructor
        int sample_per_pixel;
        int max_depth;
        double vfov;
        point3 lookfrom;
        point3 lookat;
        Vector3 vup;
        double defocus_angle;
        double focus_distance;

        void render(Adafruit_ILI9341& tft, const hittable& world) {
            // Render the scene to the display

            // Initialize the camera
            initialize(tft);

            // Render the scene
            for (int j = tft.height() - 1; j >= 0; --j) {
                for (int i = 0; i < tft.width(); ++i) {
                    Color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < sample_per_pixel; ++sample) {
                        auto u = (double(i) + random_double()) / (tft.width() - 1);
                        auto v = (double(j) + random_double()) / (tft.height() - 1);
                        ray r(camera_origin, viewport_upper_left + u * horizontal + v * vertical - camera_origin);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    pixel_color *= pixel_samples_scale;
                    writeColor(i, j, pixel_color, tft);
                }
            }
        }

    private:
        // Camera properties
        point3 camera_origin;
        Vector3 horizontal;
        Vector3 vertical;
        point3 viewport_upper_left;
        double pixel_samples_scale;
        Vector3 u, v, w;
        Vector3 defocus_disk_u;
        Vector3 defocus_disk_v;


    void initialize(Adafruit_ILI9341& tft) {
        // Calculate the camera properties
        auto aspect_ratio = tft.width() / tft.height();
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto focal_length = (lookfrom - lookat).length();
        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = aspect_ratio * viewport_height;

        // Set the camera properties
        pixel_samples_scale = 1.0 / sample_per_pixel;
        camera_origin = lookfrom;

        // Calculate the camera basis vectors
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the viewport properties
        Vector3 viewport_u = viewport_width * u;
        Vector3 viewport_v = viewport_height * -v; 

        // Calculate the viewport vectors
        horizontal = viewport_u;
        vertical = viewport_v;

        // Calculate the viewport upper left corner
        viewport_upper_left = camera_origin - (focal_length * w) - viewport_u/2 - viewport_v/2;

        // Calculate the defocus disk properties
        auto defocus_radius = focus_distance * tan(degrees_to_radians(defocus_angle / 2));

        // Calculate the defocus disk vectors
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Returns a ray from the camera origin to the viewport pixel (i, j).

        // Calculate the offset for the pixel samples
        auto offset = sample_square();

        // Calculate the ray origin
        auto ray_origin = (defocus_angle <= 0) ? camera_origin : defocus_disk_sample();

        // Calculate the ray direction        
        return ray(ray_origin, viewport_upper_left + (i + offset.x()) * horizontal + (j + offset.y()) * vertical - ray_origin);
    }

    Vector3 sample_square() const {
        // Returns a random point in the unit square.
        return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_origin + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    Color ray_color(const ray& r, int depth, const hittable& world) const {
        // Establish the base case for the recursion
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        // Check if the ray intersects the world
        hit_record rec;
        if (world.hit(r, interval(0.001, infi), rec)) {
            ray scattered;
            Color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                // Recursively calculate the scattered ray color
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return Color(0, 0, 0);
        }

        // Calculate the background color
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};  

#endif