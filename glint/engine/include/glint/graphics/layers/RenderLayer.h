#pragma once

#include <vector>

#include "vulkan/vulkan_core.h"

namespace glint::engine {
    namespace core {
        struct EntityView;
    }
}

namespace glint::engine::graphics {
    struct LayerRenderInfo {
        VkCommandBuffer commands = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        VkDescriptorSet cameraSet = nullptr;
        VkDescriptorSet entitySet = nullptr;

        const std::vector<core::EntityView>* entities;
    };

    class RenderLayer {
      protected:
        RenderLayer() = default;

      public:
        virtual ~RenderLayer() = default;

        virtual void begin() {};
        virtual void render(float deltaTime, const LayerRenderInfo& info) = 0;
        virtual void end() {};
    };

}