#pragma once

#include <memory>

#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace graphics {
        namespace models {
            struct Mesh;
        }
        namespace backend {
            struct UniformBuffer;
        }
    }
    namespace scene {
        struct World;
        namespace components {
            struct CameraSnapshot;
        }
    }
}

namespace glint::engine::graphics::layers {
    struct SceneLayer : public RenderLayer {
      private:
        const scene::World& world;
        std::unique_ptr<backend::UniformBuffer> entityBuffer;

      public:
        SceneLayer(const scene::World& world);
        ~SceneLayer();

      public:
        void init(const backend::DeviceHandles& devices) override;

        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}