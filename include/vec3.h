#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <Arduino.h>

// Define the Vector3 class
class Vector3 {
    public:

    // Define the public variables

    // The vector components
    float e[3];

    // Define the constructors
    Vector3() : e{0, 0, 0} {}
    Vector3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    // Define the x, y, and z methods
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    // Define the unary operators
    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    // Define the vector arithmetic operators
    Vector3& operator+=(const Vector3 &v) {
        // Add the vector to the current vector
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Define the vector arithmetic operators

    Vector3& operator*=(const float t) {
        // Multiply the vector by the scalar
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const float t) {
        // Multiply the vector by the reciprocal of the scalar
        return *this *= 1/t;
    }

    float length() const {
        // Return the length of the vector
        return sqrt(length_squared());
    }

    float length_squared() const {
        // Return the squared length of the vector
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Utility Functions

    static float random_float() {
        // Returns a random real in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }

    static float random_float(float min, float max) {
        // Returns a random real in [min,max).
        return min + (max-min)*random_float();
    }

    static Vector3 random() {
        //  Returns a random vector with components in [0,1).
        return Vector3(random_float(), random_float(), random_float());
    }

    static Vector3 random(float min, float max) {
        // Returns a random vector with components in [min,max).
        return Vector3(random_float(min, max), random_float(min, max), random_float(min, max));
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

// Create aliases for Vector3
using point3 = Vector3;   // 3D point

// Inline Utility Functions

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    // Add two vectors
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    // Subtract two vectors
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    // Multiply two vectors
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(float t, const Vector3 &v) {
    // Multiply a vector by a scalar
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, float t) {
    // Multiply a vector by a scalar
    return t * v;
}

inline Vector3 operator/(Vector3 v, float t) {
    // Divide a vector by a scalar
    return (1/t) * v;
}

inline float dot(const Vector3 &u, const Vector3 &v) {
    // Calculate the dot product of two vectors
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    // Calculate the cross product of two vectors
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                   u.e[2] * v.e[0] - u.e[0] * v.e[2],
                   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v) {
    // Return the unit vector of the input vector
    return v / v.length();
}

inline Vector3 random_in_unit_sphere() {
    // Returns a random vector in the unit sphere.
    while (true) {
        auto p = Vector3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vector3 random_unit_vector() {
    // Returns a random unit vector.
    return unit_vector(random_in_unit_sphere());
}

inline Vector3 random_in_unit_disk() {
    // Returns a random vector in the unit disk.
    while (true) {
        auto p = Vector3(Vector3::random_float(-1,1), Vector3::random_float(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline Vector3 random_on_hemisphere(const Vector3& normal) {
    // Returns a random vector on the hemisphere.
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    // Reflect the vector v around the normal n
    return v - 2*dot(v,n)*n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat) {
    // Refract the vector uv around the normal n
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif