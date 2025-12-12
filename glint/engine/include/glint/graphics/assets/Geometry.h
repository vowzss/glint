#pragma once

#include "Vertex.h"

namespace glint::engine::graphics {

    class Geometry {
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

      public:
        Geometry() = delete;
        ~Geometry() noexcept = default;

        Geometry(const Geometry&) = delete;
        Geometry& operator=(const Geometry&) = delete;

        Geometry(Geometry&&) noexcept = default;
        Geometry& operator=(Geometry&&) noexcept = default;

        Geometry(std::vector<Vertex> vertices, std::vector<uint32_t> indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {};

        // --- getters --
        const std::vector<Vertex>& vertices() const noexcept {
            return m_vertices;
        }

        const std::vector<uint32_t>& indices() const noexcept {
            return m_indices;
        }
    };

}