#pragma once

#include "glint/core/math/vectors/vec4.h"
#include "mat.h"

struct mat4 : public mat<float, 4, 4> {
    using mat<float, 4, 4>::mat;

    // --- constructors ---
    constexpr mat4() : mat<float, 4, 4>() {}
    constexpr mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33)
        : mat<float, 4, 4>(std::array<float, 16>{m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33}) {}
    constexpr mat4(const vec4& c0, const vec4& c1, const vec4& c2, const vec4& c3) {
        for (size_t i = 0; i < 4; ++i) {
            data[i][0] = c0[i];
            data[i][1] = c1[i];
            data[i][2] = c2[i];
            data[i][3] = c3[i];
        }
    }

    // --- factories ---
    static constexpr mat4 identity();

    // --- getters / setters ---
    constexpr float& m00() { return data[0][0]; }
    constexpr const float& m00() const { return data[0][0]; }
    constexpr float& m01() { return data[0][1]; }
    constexpr const float& m01() const { return data[0][1]; }
    constexpr float& m02() { return data[0][2]; }
    constexpr const float& m02() const { return data[0][2]; }
    constexpr float& m03() { return data[0][3]; }
    constexpr const float& m03() const { return data[0][3]; }

    constexpr float& m10() { return data[1][0]; }
    constexpr const float& m10() const { return data[1][0]; }
    constexpr float& m11() { return data[1][1]; }
    constexpr const float& m11() const { return data[1][1]; }
    constexpr float& m12() { return data[1][2]; }
    constexpr const float& m12() const { return data[1][2]; }
    constexpr float& m13() { return data[1][3]; }
    constexpr const float& m13() const { return data[1][3]; }

    constexpr float& m20() { return data[2][0]; }
    constexpr const float& m20() const { return data[2][0]; }
    constexpr float& m21() { return data[2][1]; }
    constexpr const float& m21() const { return data[2][1]; }
    constexpr float& m22() { return data[2][2]; }
    constexpr const float& m22() const { return data[2][2]; }
    constexpr float& m23() { return data[2][3]; }
    constexpr const float& m23() const { return data[2][3]; }

    constexpr float& m30() { return data[3][0]; }
    constexpr const float& m30() const { return data[3][0]; }
    constexpr float& m31() { return data[3][1]; }
    constexpr const float& m31() const { return data[3][1]; }
    constexpr float& m32() { return data[3][2]; }
    constexpr const float& m32() const { return data[3][2]; }
    constexpr float& m33() { return data[3][3]; }
    constexpr const float& m33() const { return data[3][3]; }
};

#include "mat4.inl"
