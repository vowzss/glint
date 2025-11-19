#pragma once

#include <vector>

struct vertex;

struct mesh {
    std::vector<vertex> vertices;
    std::vector<int> indices;

    mesh() = default;
    mesh(const std::vector<vertex>& verts, const std::vector<int>& inds)
        : vertices(verts), indices(inds) {}
};