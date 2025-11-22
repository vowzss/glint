#pragma once

#include "vec2.h"

inline float vec<float, 2>::dot(const vec<float, 2>& v) const {
    return x * v.x + y * v.y;
}

inline float vec<float, 2>::length() const {
    return std::sqrt(x * x + y * y);
}

inline vec<float, 2> vec<float, 2>::normalized() const {
    float inv = 1.0f / length();
    return {x * inv, y * inv};
}

inline vec<float, 2> vec<float, 2>::operator+(const vec<float, 2>& v) const {
    return {x + v.x, y + v.y};
}

inline vec<float, 2> vec<float, 2>::operator-(const vec<float, 2>& v) const {
    return {x - v.x, y - v.y};
}

inline vec<float, 2> vec<float, 2>::operator*(float s) const {
    return {x * s, y * s};
}
