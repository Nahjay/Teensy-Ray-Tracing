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
#include "camera.h"

// Useful stuff from C++ Standard Library
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

#endif
