#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "GeometryHandle.h"

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext;
        struct GeometryBuffer;
    }
}

namespace glint::engine::core {
    struct AssetManager;

    struct GeometryManager {
      private:
        using Devices = graphics::backend::DeviceContext;
        using GeometryBuffer = graphics::backend::GeometryBuffer;

      private:
        AssetManager* assets;

        std::vector<std::shared_ptr<GeometryBuffer>> entries;

        std::vector<uint32_t> freeIds;
        uint32_t nextId = 0;

      public:
        GeometryManager(AssetManager* assets_) : assets(assets_) {};
        ~GeometryManager() = default;

      public:
        GeometryHandle create(const Devices& devices, const std::string& path);
        void destroy(GeometryHandle handle);

      private:
        uint32_t computeUniqueId();
    };

}