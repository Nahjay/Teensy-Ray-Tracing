#ifndef CAMERA_H
#define CAMERA_H

#include "ray_tracing.h"
#include "hittable.h"
#include <Adafruit_ILI9341.h>
#include <limits>

const double infi = std::numeric_limits<double>::infinity();

class camera {
    public:
        int sample_per_pixel;

        void render(Adafruit_ILI9341& tft, const hittable& world) {
            initialize(tft);

            for (int j = tft.height() - 1; j >= 0; --j) {
                for (int i = 0; i < tft.width(); ++i) {
                    Color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < sample_per_pixel; ++sample) {
                        auto u = (double(i) + random_double()) / (tft.width() - 1);
                        auto v = (double(j) + random_double()) / (tft.height() - 1);
                        ray r(camera_origin, viewport_upper_left + u * horizontal + v * vertical - camera_origin);
                        pixel_color += ray_color(r, world);
                    }
                    pixel_color *= pixel_samples_scale;
                    writeColor(i, j, pixel_color, tft);
                }
            }
        }

    private:
        point3 camera_origin;
        Vector3 horizontal;
        Vector3 vertical;
        point3 viewport_upper_left;
        double pixel_samples_scale;

    void initialize(Adafruit_ILI9341& tft) {
        auto aspect_ratio = tft.width() / tft.height();
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        pixel_samples_scale = 1.0 / sample_per_pixel;
        camera_origin = point3(0, 0, 0);
        horizontal = Vector3(viewport_width, 0, 0);
        vertical = Vector3(0, -viewport_height, 0);
        viewport_upper_left = camera_origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, 1);
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        return ray(camera_origin, viewport_upper_left + (i + offset.x()) * horizontal + (j + offset.y()) * vertical - camera_origin);
    }

    Vector3 sample_square() const {
        return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, interval(0, infi), rec)) {
            Vector3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.p, direction), world);
            // return 0.5 * Color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
        }
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};  

#endif