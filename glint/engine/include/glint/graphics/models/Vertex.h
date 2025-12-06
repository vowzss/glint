#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Float2.h>
#include <Jolt/Math/Float3.h>

namespace glint::engine::graphics::models {

    struct alignas(16) Vertex {
        JPH::Float3 position;
        JPH::Float3 normal;
        JPH::Float3 color;
        JPH::Float2 uv;

        Vertex(JPH::Float3 p, JPH::Float3 c, JPH::Float3 n, JPH::Float2 t)
            : position(p), color(c), normal(n), uv(t) {
        }

        // --- operators ---
        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color && normal == other.normal
                   && uv == other.uv;
        }
    };

}

namespace std {
    template <>
    struct hash<glint::engine::graphics::models::Vertex> {
        size_t operator()(const glint::engine::graphics::models::Vertex& v) const noexcept {
            size_t h1 = std::hash<float>()(v.position.x) ^ (std::hash<float>()(v.position.y) << 1)
                        ^ (std::hash<float>()(v.position.z) << 2);
            size_t h2 = std::hash<float>()(v.color.x) ^ (std::hash<float>()(v.color.y) << 1)
                        ^ (std::hash<float>()(v.color.z) << 2);
            size_t h3 = std::hash<float>()(v.normal.x) ^ (std::hash<float>()(v.normal.y) << 1)
                        ^ (std::hash<float>()(v.normal.z) << 2);
            size_t h4 = std::hash<float>()(v.uv.x) ^ (std::hash<float>()(v.uv.y) << 1);
            return h1 ^ h2 ^ h3 ^ h4;
        }
    };
}