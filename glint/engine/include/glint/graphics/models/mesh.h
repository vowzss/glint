#pragma once

#include <vector>

#include "Vertex.h"

namespace glint::engine::graphics {
    namespace models {
        struct Mesh {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            Mesh() = delete;

            Mesh(const std::vector<Vertex>& vertices_, const std::vector<uint32_t>& indices_) : vertices(vertices_), indices(indices_) {}
            Mesh(std::vector<Vertex>&& vertices_, std::vector<uint32_t>&& indices_) noexcept
                : vertices(std::move(vertices_)), indices(std::move(indices_)) {}
        };
    }
}