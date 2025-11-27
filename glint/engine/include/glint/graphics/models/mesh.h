#pragma once

#include <vector>

namespace glint::engine::graphics::models {
    struct Vertex;

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Mesh() = default;
        Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds) : vertices(verts), indices(inds) {}

        Mesh(std::vector<Vertex>&& verts, std::vector<uint32_t>&& inds) noexcept : vertices(std::move(verts)), indices(std::move(inds)) {}
    };
}