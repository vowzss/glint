#pragma once

#include <memory>

#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
    }
    namespace graphics {
        struct Mesh;
        struct StorageBuffer;
    }
    namespace scene {
        struct World;
    }
}

namespace glint::engine::graphics {

    struct SceneLayer : public RenderLayer {
      private:
        const scene::World& world;
        std::unique_ptr<StorageBuffer> entityBuffer;

      public:
        SceneLayer(const scene::World& world);
        ~SceneLayer();

        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}