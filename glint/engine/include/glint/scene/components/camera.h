#pragma once

#include "glint/core/math/matrix/mat4.h"
#include "glint/core/math/vector/vec3.h"

namespace glint::engine::scene::components {
    using vec3 = glint::engine::core::math::vector::vec3;
    using mat4 = glint::engine::core::math::matrix::mat4;

    struct camera {
        vec3 position;
        vec3 front;
        vec3 up;

        float fov;
        float aspect;
        float near;
        float far;

        camera(vec3 pos = vec3(0, 0, 0), vec3 front = vec3(0, 0, -1), vec3 up = vec3(0, 1, 0), float fov = 45.0f, float aspect = 16.0f / 9.0f,
            float near = 0.1f, float far = 100.0f)
            : position(pos), front(front), up(up), fov(fov), aspect(aspect), near(near), far(far) {}

        mat4 getViewMatrix() const { return lookAt(position, position + front, up); }

      private:
        mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) const;
    };
}