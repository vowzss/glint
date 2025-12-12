#include <filesystem>
#include <tiny_obj_loader.h>

#include "glint/core/Logger.h"
#include "glint/graphics/assets/Geometry.h"
#include "glint/graphics/assets/GeometryLoader.h"
#include "glint/graphics/assets/Vertex.h"

namespace glint::engine::graphics {

    std::optional<Geometry> GeometryLoader::load(const std::string& filename) {
        namespace fs = std::filesystem;

        LOG_INFO("Loading OBJ file from: {}", filename);

        if (!fs::exists(filename) || fs::file_size(filename) == 0) {
            LOG_ERROR("OBJ file is missing or empty: {}", filename);
            return std::nullopt;
        }

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
            LOG_ERROR("Failed to load OBJ: {}", err);
            return std::nullopt;
        }

        std::vector<Vertex> vertices = {};
        std::vector<uint32_t> indices = {};

        for (const tinyobj::shape_t& shape : shapes) {
            for (const tinyobj::index_t& index : shape.mesh.indices) {
                // clang-format off
                    Vertex vertex{
                        {   
                            attrib.vertices[3 * index.vertex_index + 0], 
                            attrib.vertices[3 * index.vertex_index + 1], 
                            attrib.vertices[3 * index.vertex_index + 2]
                        },
                        {
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 0] : 0.0f,
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 1] : 0.0f,
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 2] : 0.0f
                        },
                        {1.0f, 1.0f, 1.0f},
                        {
                            index.texcoord_index >= 0 ? attrib.texcoords[2 * index.texcoord_index + 0] : 0.0f,
                            index.texcoord_index >= 0 ? attrib.texcoords[2 * index.texcoord_index + 1] : 0.0f
                        }
                    };
                // clang-format on
                vertices.emplace_back(vertex);
                indices.emplace_back(static_cast<uint32_t>(indices.size()));
            }
        }

        return Geometry{vertices, indices};
    }

}