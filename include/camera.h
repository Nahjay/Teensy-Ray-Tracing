#ifndef CAMERA_H
#define CAMERA_H

#include "ray_tracing.h"
#include "hittable.h"
#include <Adafruit_ILI9341.h>

class camera {
    public:
        void render(Adafruit_ILI9341& tft, const hittable& world) {
            for (int j = tft.height() - 1; j >= 0; --j) {
                for (int i = 0; i < tft.width(); ++i) {
                    auto u = double(i) / (tft.width() - 1);
                    auto v = double(j) / (tft.height() - 1);
                    ray r(camera_origin, viewport_upper_left + u * horizontal + v * vertical - camera_origin);
                    Color pixel_color = ray_color(r, world);
                    writeColor(i, j, pixel_color, tft);
                }
            }
        }

    private:
        point3 camera_origin;
        Vector3 horizontal;
        Vector3 vertical;
        point3 viewport_upper_left;

    void initialize(Adafruit_ILI9341& tft) {
        auto aspect_ratio = tft.width() / tft.height();
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        camera_origin = point3(0, 0, 0);
        horizontal = Vector3(viewport_width, 0, 0);
        vertical = Vector3(0, -viewport_height, 0);
        viewport_upper_left = camera_origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, 1);
    }

    Color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, interval(0, inf), rec)) {
            return 0.5 * Color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
        }
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};  

#endif