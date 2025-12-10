#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "GeometryHandle.h"

namespace glint::engine::graphics {
    struct Devices;
    struct GeometryBuffer;
}

namespace glint::engine::core {

    struct AssetManager;

    struct GeometryManager {
      private:
        using Devices = graphics::Devices;
        using GeometryBuffer = graphics::GeometryBuffer;

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