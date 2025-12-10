#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Float2.h>
#include <Jolt/Math/Float3.h>

namespace glint::engine::graphics {

    struct alignas(16) Vertex {
      public:
        JPH::Float3 position;
        JPH::Float3 normal;
        JPH::Float3 color;
        JPH::Float2 uv;

      public:
        Vertex() = delete;

        constexpr Vertex(JPH::Float3 p, JPH::Float3 c, JPH::Float3 n, JPH::Float2 t) : position(p), color(c), normal(n), uv(t) {
        }

      public:
        bool operator==(const Vertex& other) const noexcept {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

}