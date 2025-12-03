#pragma once

#include <filesystem>
#include <memory>

namespace glint::engine {
    namespace graphics::models {
        struct Mesh;
    }

    namespace loaders {
        struct ModelLoader {
            std::shared_ptr<graphics::models::Mesh> load(const std::string& filename);
        };
    }
}