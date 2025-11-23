#pragma once

#include "glint/core/math/vectors/vec2.h"
#include "mat.h"

struct mat2 : public mat<float, 2, 2> {
    using mat<float, 2, 2>::mat;

    // --- constructors ---
    constexpr mat2() : mat<float, 2, 2>() {}
    constexpr mat2(float m00, float m01, float m10, float m11) : mat<float, 2, 2>({m00, m01, m10, m11}) {}
    constexpr mat2(const vec2& c0, const vec2& c1) {
        for (size_t i = 0; i < 2; ++i) {
            data[i][0] = c0[i];
            data[i][1] = c1[i];
        }
    }

    // --- factories ---
    static constexpr mat2 identity();

    // --- getters / setters ---
    constexpr float& m00() { return data[0][0]; }
    constexpr const float& m00() const { return data[0][0]; }
    constexpr float& m01() { return data[0][1]; }
    constexpr const float& m01() const { return data[0][1]; }

    constexpr float& m10() { return data[1][0]; }
    constexpr const float& m10() const { return data[1][0]; }
    constexpr float& m11() { return data[1][1]; }
    constexpr const float& m11() const { return data[1][1]; }
};

#include "mat2.inl"
