#include "glint/scene/components/camera.h"

namespace glint::engine::scene::components {
    namespace {
        mat4 perspective(float fov, float aspect, float near, float far) {
            float fovRad = fov * 3.14159265f / 180.0f;
            float tanHalfFov = std::tan(fovRad / 2.0f);

            mat4 result{};
            result[0][0] = 1.0f / (aspect * tanHalfFov);
            result[1][1] = 1.0f / tanHalfFov;

            result[2][2] = far / (far - near);
            result[2][3] = 1.0f;

            result[3][2] = -(far * near) / (far - near);
            result[3][3] = 0.0f;

            return result;
        }

        mat4 orthographic(float left, float right, float bottom, float top, float near, float far) {
            mat4 result{};
            result[0][0] = 2.0f / (right - left);

            result[1][1] = 2.0f / (top - bottom);

            result[2][2] = 2.0f / (far - near);

            result[3][0] = -(right + left) / (right - left);
            result[3][1] = -(top + bottom) / (top - bottom);
            result[3][2] = -(far + near) / (far - near);
            result[3][3] = 1.0f;

            return result;
        }
    }

    mat4 camera::lookAt(const vec3& eye, const vec3& center, const vec3& up) const {
        vec3 f = (center - eye).normalize();
        vec3 s = f.cross(up).normalize();
        vec3 u = s.cross(f);

        mat4 result;
        // clang-format off
        result[0][0] = s.x();  result[0][1] = u.x();  result[0][2] = -f.x();  result[0][3] = 0.0f;
        result[1][0] = s.y();  result[1][1] = u.y();  result[1][2] = -f.y();  result[1][3] = 0.0f;
        result[2][0] = s.z();  result[2][1] = u.z();  result[2][2] = -f.z();  result[2][3] = 0.0f;
        result[3][0] = -s.dot(eye); 
        result[3][1] = -u.dot(eye); 
        result[3][2] = f.dot(eye); 
        result[3][3] = 1.0f;
        // clang-format on
        return result;
    }

    mat4 camera::getViewMatrix() const {
        return lookAt(position, position + front, up);
    }

    mat4 camera::getProjectionMatrix() const {
        mat4 proj = perspective(fov, aspect, near, far);
        proj[1][1] *= -1.0f;

        return proj;
    }
}