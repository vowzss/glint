#pragma once

#include "vec.h"

struct vec2 : public vec<float, 2, vec2> {
    using vec<float, 2, vec2>::vec;

    // --- constructors ---
    constexpr vec2() : vec<float, 2, vec2>(0.0f) {}
    constexpr vec2(float x, float y) : vec<float, 2, vec2>({x, y}) {}

    // --- getters / setters ---
    constexpr float& x() { return data[0]; }
    constexpr const float& x() const { return data[0]; }

    constexpr float& y() { return data[1]; }
    constexpr const float& y() const { return data[1]; }
};

struct vec2i : public vec<int, 2, vec2i> {
    using vec<int, 2, vec2i>::vec;

    // --- constructors ---
    inline vec2i() : vec<int, 2, vec2i>(0) {}
    constexpr vec2i(int x, int y) : vec<int, 2, vec2i>({x, y}) {}

    // --- getters / setters ---
    constexpr int& x() { return data[0]; }
    constexpr const int& x() const { return data[0]; }

    constexpr int& y() { return data[1]; }
    constexpr const int& y() const { return data[1]; }
};

#include "vec2.inl"