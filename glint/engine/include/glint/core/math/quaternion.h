#pragma once

#include "vector/vec3.h"

namespace glint::engine::core::math {
    struct quaternion {
        float w, x, y, z;

        // --- constructors ---
        constexpr quaternion() : w(1), x(0), y(0), z(0) {}
        constexpr quaternion(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_) {}
        inline quaternion(float angle, const vector::vec3& axis) {
            float half = angle * 0.5f;
            float s = std::sin(half);

            w = std::cos(half);
            x = axis.x() * s;
            y = axis.y() * s;
            z = axis.z() * s;
        }

        // --- methods ---
        quaternion normalized() const {
            float len = std::sqrt(w * w + x * x + y * y + z * z);
            return {w / len, x / len, y / len, z / len};
        }

        quaternion& normalize() {
            float len = std::sqrt(w * w + x * x + y * y + z * z);
            w /= len;
            x /= len;
            y /= len;
            z /= len;
            return *this;
        }

        // --- operators (combine) ---
        quaternion operator*(const quaternion& r) const {
            return {w * r.w - x * r.x - y * r.y - z * r.z, w * r.x + x * r.w + y * r.z - z * r.y, w * r.y - x * r.z + y * r.w + z * r.x,
                w * r.z + x * r.y - y * r.x + z * r.w};
        }
    };
}