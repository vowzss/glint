#pragma once

#include "vec3.h"

struct vec4 : public vec<float, 4, vec4> {
    using vec<float, 4, vec4>::vec;

    // --- constructors ---
    constexpr vec4() : vec<float, 4, vec4>(0) {}
    constexpr vec4(float x, float y, float z, float w) : vec<float, 4, vec4>({x, y, z, w}) {}
    constexpr vec4(vec3 v, float w = 1.0f) : vec<float, 4, vec4>({v.x(), v.y(), v.z(), w}) {}

    // --- getters / setters ---
    constexpr float& x() { return data[0]; }
    constexpr const float& x() const { return data[0]; }

    constexpr float& y() { return data[1]; }
    constexpr const float& y() const { return data[1]; }

    constexpr float& z() { return data[2]; }
    constexpr const float& z() const { return data[2]; }

    constexpr float& w() { return data[3]; }
    constexpr const float& w() const { return data[3]; }
};

struct vec4i : public vec<int, 4, vec4i> {
    using vec<int, 4, vec4i>::vec;

    // --- constructors ---
    constexpr vec4i() : vec<int, 4, vec4i>(0) {}
    constexpr vec4i(int x, int y, int z, int w) : vec<int, 4, vec4i>({x, y, z, w}) {}
    constexpr vec4i(vec3i v, int w = 1) : vec<int, 4, vec4i>({v.x(), v.y(), v.z(), w}) {}

    // --- getters / setters ---
    constexpr int& x() { return data[0]; }
    constexpr const int& x() const { return data[0]; }

    constexpr int& y() { return data[1]; }
    constexpr const int& y() const { return data[1]; }

    constexpr int& z() { return data[2]; }
    constexpr const int& z() const { return data[2]; }

    constexpr int& w() { return data[3]; }
    constexpr const int& w() const { return data[3]; }
};

#include "vec4.inl"
