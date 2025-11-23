#pragma once

#include "../vectors/vec2.h"
#include "mat.h"

struct mat2 : public mat<float, 2, 2, mat2> {
    using mat<float, 2, 2, mat2>::mat;

    // --- constructors ---
    constexpr mat2() : mat<float, 2, 2, mat2>() {}
    constexpr mat2(const mat<float, 2, 2, mat2>& m) : mat<float, 2, 2, mat2>(m) {}

    constexpr mat2(float m00, float m01, float m10, float m11) : mat<float, 2, 2, mat2>({m00, m01, m10, m11}) {}
    constexpr mat2(const vec2& c0, const vec2& c1) {
        for (size_t i = 0; i < 2; ++i) {
            data[i][0] = c0[i];
            data[i][1] = c1[i];
        }
    }

    // --- factories ---
    static constexpr mat2 identity();
};

#include "mat2.inl"
