#pragma once

#include "Vertex.h"

namespace glint::engine::graphics {

    class GeometryData {
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

      public:
        GeometryData() noexcept = default;
        ~GeometryData() noexcept = default;

        GeometryData(const GeometryData&) = delete;
        GeometryData& operator=(const GeometryData&) = delete;

        GeometryData(GeometryData&&) noexcept = default;
        GeometryData& operator=(GeometryData&&) noexcept = default;

        GeometryData(std::vector<Vertex> vertices, std::vector<uint32_t> indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {};

        // --- getters --
        const std::vector<Vertex>& vertices() const noexcept {
            return m_vertices;
        }

        const std::vector<uint32_t>& indices() const noexcept {
            return m_indices;
        }
    };

}