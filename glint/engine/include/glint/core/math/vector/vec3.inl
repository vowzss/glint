#pragma once

namespace glint::engine::core::math::vector {
    // --- methods ---
    constexpr vec3 vec3::cross(const vec3& v) const {
        return vec3(data[1] * v.data[2] - data[2] * v.data[1], data[2] * v.data[0] - data[0] * v.data[2], data[0] * v.data[1] - data[1] * v.data[0]);
    }

    inline float vec3::angle(const vec3& v) const {
        float dotProd = this->dot(v);
        float lenProd = this->length() * v.length();

        if (lenProd < 1e-6f) {
            return 0.0f;
        }

        float cosTheta = dotProd / lenProd;
        if (cosTheta > 1.0f) cosTheta = 1.0f;
        if (cosTheta < -1.0f) cosTheta = -1.0f;
        return std::acos(cosTheta);
    }

    // --- methods ---
    constexpr vec3i vec3i::cross(const vec3i& v) const {
        return vec3i(data[1] * v.data[2] - data[2] * v.data[1], data[2] * v.data[0] - data[0] * v.data[2], data[0] * v.data[1] - data[1] * v.data[0]);
    }

    inline float vec3i::angle(const vec3i& v) const {
        float dotProd = this->dot(v);
        float lenProd = this->length() * v.length();

        if (lenProd < 1e-6f) {
            return 0.0f;
        }

        float cosTheta = dotProd / lenProd;
        if (cosTheta > 1.0f) cosTheta = 1.0f;
        if (cosTheta < -1.0f) cosTheta = -1.0f;
        return std::acos(cosTheta);
    }
}