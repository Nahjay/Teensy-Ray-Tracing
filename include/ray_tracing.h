// General Header File

#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <cmath>
#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <limits>
#include <memory>
#include <cstdlib>

// Common Headers
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "interval.h"

// Useful stuff from C++ Standard Library
using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

// Constants
const float inf = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

// Degrees to Radians
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

// Random Number Generation
inline float random_float() {
    return rand() / (RAND_MAX + 1.0);
}

// Random Number Generation with Range
inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}

#endif
