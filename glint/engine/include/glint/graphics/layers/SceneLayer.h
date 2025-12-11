#pragma once

#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
    }
    namespace graphics {
        struct StorageBuffer;
    }
    namespace scene {
        class World;
    }
}

namespace glint::engine::graphics {

    class SceneLayer : public RenderLayer {
        const scene::World& m_world;

      public:
        SceneLayer(const scene::World& world);
        ~SceneLayer();

        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}