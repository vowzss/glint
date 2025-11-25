#pragma once

#include "../quaternion.h"
#include "../vector/vec3.h"
#include "../vector/vec4.h"
#include "mat.h"


namespace glint::engine::core::math::matrix {
    struct mat4 : public mat<float, 4, 4, mat4> {
        using mat<float, 4, 4, mat4>::mat;

        // --- constructors ---
        constexpr mat4() : mat<float, 4, 4, mat4>() {}
        constexpr mat4(const mat<float, 4, 4, mat4>& m) : mat<float, 4, 4, mat4>(m) {}

        constexpr mat4(const vector::vec4& c0, const vector::vec4& c1, const vector::vec4& c2, const vector::vec4& c3) {
            for (size_t i = 0; i < 4; ++i) {
                data[i][0] = c0[i];
                data[i][1] = c1[i];
                data[i][2] = c2[i];
                data[i][3] = c3[i];
            }
        }

        // --- factories ---
        static constexpr mat4 identity();

        static mat4 translate(const vector::vec3& val);
        static mat4 rotate(const quaternion& val);
        static mat4 scale(const vector::vec3& val);
    };
}

#include "mat4.inl"
