#pragma once

#include "Vertex.h"

namespace glint::engine::graphics::models {
    struct GeometryData {
      private:
        using vertices_t = std::vector<Vertex>;
        using indices_t = std::vector<uint32_t>;

      public:
        vertices_t vertices;
        indices_t indices;

      public:
        GeometryData() = default;
        GeometryData(const GeometryData&) = delete;
        GeometryData(GeometryData&&) noexcept = default;

        GeometryData(vertices_t vertices_, indices_t indices_) : vertices(std::move(vertices_)), indices(std::move(indices_)) {
        }

        GeometryData& operator=(const GeometryData&) = delete;
        GeometryData& operator=(GeometryData&&) noexcept = default;
    };
}