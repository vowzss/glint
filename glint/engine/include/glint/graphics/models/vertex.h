#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Float2.h>
#include <Jolt/Math/Float3.h>

namespace glint::engine::graphics::models {
    struct alignas(16) Vertex {
        JPH::Float3 pos;
        JPH::Float3 color;
        JPH::Float3 normal;
        JPH::Float2 uv;

        Vertex(JPH::Float3 p, JPH::Float3 c, JPH::Float3 n, JPH::Float2 t) : pos(p), color(c), normal(n), uv(t) {}
    };
}