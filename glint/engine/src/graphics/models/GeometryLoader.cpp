#include <filesystem>
#include <tiny_obj_loader.h>

#include "glint/core/Logger.h"
#include "glint/graphics/models/GeometryData.h"
#include "glint/graphics/models/GeometryLoader.h"
#include "glint/graphics/models/Vertex.h"
#include "glint/utils/FileUtils.h"

namespace glint::engine::graphics::models {

    std::optional<GeometryData> GeometryLoader::load(const std::string& filename) {
        namespace fs = std::filesystem;
        using namespace graphics::models;

        fs::path path = utils::files::getModelPath(filename);
        LOG_INFO("Loading OBJ file from: {}", path.string());

        if (!fs::exists(path) || fs::file_size(path) == 0) {
            LOG_ERROR("OBJ file is missing or empty: {}", path.string());
            return std::nullopt;
        }

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str())) {
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
                        {1.0f, 1.0f, 1.0f},
                        {
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 0] : 0.0f,
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 1] : 0.0f,
                            index.normal_index >= 0 ? attrib.normals[3 * index.normal_index + 2] : 0.0f
                        },
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

        return GeometryData{vertices, indices};
    }

}