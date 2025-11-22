#pragma once

#include "vec.h"

template <> struct vec<float, 2> {
    union {
        struct {
            float x, y;
        };
        float data[2];
    };

    inline vec()
        : x(0), y(0) {
    }

    inline vec(float x, float y)
        : x(x), y(y) {
    }

    inline explicit vec(float v)
        : x(v), y(v) {
    }

    // ---  ---
    inline float length() const;
    inline float dot(const vec& v) const;
    inline vec normalized() const;

    // --- operator ---
    inline float& operator[](size_t i) {
        return data[i];
    }
    inline const float& operator[](size_t i) const {
        return data[i];
    }
    inline vec operator+(const vec& v) const;
    inline vec operator-(const vec& v) const;
    inline vec operator*(float s) const;
};

#include "vec2.inl"
