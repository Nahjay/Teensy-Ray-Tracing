#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracing.h"

class hit_record;

class material {
    public:
        virtual ~material() = default;
        virtual bool scatter(const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered) const = 0;
};


#endif