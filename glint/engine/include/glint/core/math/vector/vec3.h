#pragma once

#include "vec2.h"

namespace glint::engine::core::math::vector {
    struct vec3 : public vec<float, 3, vec3> {
        using vec<float, 3, vec3>::vec;

        // --- constructors ---
        inline vec3() : vec<float, 3, vec3>(0.0f) {}
        constexpr vec3(float x, float y, float z) : vec<float, 3, vec3>({x, y, z}) {}
        constexpr vec3(vec2 v, float z = 1.0f) : vec<float, 3, vec3>({v.x(), v.y(), z}) {}

        // --- factories ---
        static constexpr vec3 zero() { return vec3(0.0f, 0.0f, 0.0f); }
        static constexpr vec3 one() { return vec3(1.0f, 1.0f, 1.0f); }

        // --- methods ---
        constexpr vec3 cross(const vec3& v) const;
        inline float angle(const vec3& v) const;

        // --- getters / setters ---
        constexpr float& x() { return data[0]; }
        constexpr const float& x() const { return data[0]; }

        constexpr float& y() { return data[1]; }
        constexpr const float& y() const { return data[1]; }

        constexpr float& z() { return data[2]; }
        constexpr const float& z() const { return data[2]; }
    };

    struct vec3i : public vec<int, 3, vec3i> {
        using vec<int, 3, vec3i>::vec;

        // --- constructors ---
        inline vec3i() : vec<int, 3, vec3i>(0) {}
        constexpr vec3i(int x, int y, int z) : vec<int, 3, vec3i>({x, y, z}) {}
        constexpr vec3i(vec2i v, int z = 1) : vec<int, 3, vec3i>({v.x(), v.y(), z}) {}

        // --- methods ---
        constexpr vec3i cross(const vec3i& v) const;
        inline float angle(const vec3i& v) const;

        // --- getters / setters ---
        constexpr int& x() { return data[0]; }
        constexpr const int& x() const { return data[0]; }

        constexpr int& y() { return data[1]; }
        constexpr const int& y() const { return data[1]; }

        constexpr int& z() { return data[2]; }
    };
}

#include "vec3.inl"