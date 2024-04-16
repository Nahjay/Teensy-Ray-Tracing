#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <Arduino.h>

class Vector3 {
    public:
    double e[3];

    Vector3() : e{0, 0, 0} {}
    Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    double random_double() {
        return rand() / (RAND_MAX + 1.0);
    }


    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vector3& operator+=(const Vector3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

// Create aliases for Vector3
using Point3 = Vector3;   // 3D point
using Color = Vector3;    // RGB color

// Utility Functions

// inline functions are functions that are defined in a header file and are meant to be inlined by the compiler.
// Inlining is an optimization that eliminates the function call overhead by inserting the code of the function directly into the caller's code.
// Inlining is not always beneficial, as it can increase the size of the compiled code.
// In general, inlining is beneficial for small functions that are called frequently.

// Inline function to output the vector using the Serial Monitor
inline void outputVector(const Vector3& v) {
    Serial.print("(");
    Serial.print(v.x());
    Serial.print(", ");
    Serial.print(v.y());
    Serial.print(", ");
    Serial.print(v.z());
    Serial.println(")");
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                   u.e[2] * v.e[0] - u.e[0] * v.e[2],
                   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v) {
    return v / v.length();
}

// struct Vector3 {
//   float x, y, z;

//   // Vector addition
//   Vector3 operator +(const Vector3& v) const {
//     return {x + v.x, y + v.y, z + v.z};
//   }

//   // Vector subtraction
//   Vector3 operator -(const Vector3& v) const {
//     return {x - v.x, y - v.y, z - v.z};
//   }

//   // Scalar multiplication
//   Vector3 operator *(float scalar) const {
//     return {x * scalar, y * scalar, z * scalar};
//   }

//   // Dot product
//   float dot(const Vector3& v) const {
//     return x * v.x + y * v.y + z * v.z;
//   }

//   // Length of the vector
//   float length() const {
//     return sqrt(x * x + y * y + z * z);
//   }

//   // Cross product
//   Vector3 cross(const Vector3& v) const {
//     return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
//   }

//   // Normalize the vector
//   Vector3 normalize() const {
//     float len = sqrt(x * x + y * y + z * z);
//     return {x / len, y / len, z / len};
//   }
// };


#endif