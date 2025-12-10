#pragma once

#include "Vertex.h"

namespace glint::engine::graphics {
    struct GeometryData {
        using vertices_t = std::vector<Vertex>;
        using indices_t = std::vector<uint32_t>;

        vertices_t m_vertices;
        indices_t m_indices;

      public:
        GeometryData() = default;
        GeometryData(const GeometryData&) = delete;
        GeometryData(GeometryData&&) noexcept = default;

        GeometryData(vertices_t vertices, indices_t indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {
        }

        GeometryData& operator=(const GeometryData&) = delete;
        GeometryData& operator=(GeometryData&&) noexcept = default;
    };
}