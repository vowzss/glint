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

    class SceneLayer : public RenderLayer {
        const scene::World& world;

      public:
        SceneLayer(const scene::World& world);
        ~SceneLayer();

        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}