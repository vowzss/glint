#pragma once

#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
        class World;
    }
}

namespace glint::engine::graphics {
    struct StorageBuffer;

    class SceneLayer : public RenderLayer {
        const core::World& m_world;

      public:
        SceneLayer(const core::World& world);
        ~SceneLayer();

        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}