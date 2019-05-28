#pragma once

#include <cmath>
#include "Mat4.h"
#include "Quat.h"

namespace arc
{
    struct Mat4;
    struct Vec3
    {
        float x{};
        float y{};
        float z{};

        Vec3() {}

        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        bool isZero() {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        }

        float dot(Vec3& vector) {

            return x * vector.x + y * vector.y + z * vector.z;
        }

        Vec3 nor() {
            float len2 = Vec3::len2();
            if (len2 == 0.0f || len2 == 1.0f)
                return Vec3(x, y, z);

            float scalar = 1.0f / std::sqrt(len2);

            return Vec3(x * scalar, y * scalar, z * scalar);
        }

        Vec3 crs(Vec3& vector) {
            return Vec3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
        }

        float len2() {
            return x * x + y * y + z * z;
        }

        Vec3 operator-() const {
            return Vec3(-x, -y, -z);
        }

        Vec3 operator+(const Vec3& v) const {
            return Vec3(x + v.x, y + v.y, z + v.z);
        }

        Vec3 operator-(const Vec3& v) const {
            return Vec3(x - v.x, y - v.y, z - v.z);
        }

        Vec3 operator*(const float& v) const {
            return Vec3(x * v, y * v, z * v);
        }

        Vec3 operator/(const float& v) const {
            return Vec3(x / v, y / v, z / v);
        }

        Vec3& operator=(const Vec3& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        Vec3& operator+=(const Vec3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vec3& operator*=(const Vec3& rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        Vec3& operator*=(const float& rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Vec3& operator/=(const Vec3& rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        bool operator==(const Vec3& v) const {
            return x == v.x && y == v.y && z == v.z;
        }

        bool operator!=(const Vec3& v) const {
            return x != v.x || y != v.y || z != v.z;
        }

        static float len(Vec3 v) {
            return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        }

        static Vec3 cross(Vec3 lhs, Vec3 rhs)
        {
            Vec3 res;
            res.x = lhs.y * rhs.z - lhs.z * rhs.y;
            res.y = lhs.z * rhs.x - lhs.x * rhs.z;
            res.z = lhs.x * rhs.y - lhs.y * rhs.x;
            return res;
        }

        static Vec3 rotate(Vec3 lhs, Vec3 axis, float angle);

        static Vec3 transform(Vec3 lhs, Mat4 matrix);

        static Vec3 project(Vec3 lhs, Mat4 matrix);
    };

}

