#pragma once

#include <vector>

namespace glint::engine::graphics::models {
    struct vertex;

    struct mesh {
        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        mesh() = default;
        mesh(const std::vector<vertex>& verts, const std::vector<uint32_t>& inds) : vertices(verts), indices(inds) {}

        mesh(std::vector<vertex>&& verts, std::vector<uint32_t>&& inds) noexcept : vertices(std::move(verts)), indices(std::move(inds)) {}
    };
}