#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace glint::engine {
    namespace graphics::backend {
        struct DeviceContext;
    }
    namespace loaders {
        struct GeometryLoader;
    }
    namespace scene::components {
        struct GeometryCreateInfo;
        struct GeometryComponent;
    }
}

namespace glint::engine::core {

    struct GeometryHandle {
        uint32_t id;
        uint32_t version;

      public:
        GeometryHandle() = delete;
        GeometryHandle(uint32_t id_, uint32_t version_) : id(id_), version(version_) {};

        bool operator==(const GeometryHandle& other) const {
            return id == other.id && version == other.version;
        }
    };

    struct GeometryManager {
      private:
        using GeometryCreateInfo = scene::components::GeometryCreateInfo;

      private:
        std::unique_ptr<loaders::GeometryLoader> loader;

        std::vector<uint32_t> freeIds;
        uint32_t nextId = 0;

      public:
        GeometryManager();
        ~GeometryManager();

        /* Loads geometry data from disk into CPU memory*/
        std::optional<GeometryHandle> load(const std::string& filename);

        void destroy(GeometryHandle handle);
    };
}