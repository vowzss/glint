#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine {
    namespace graphics {
        struct UniformBuffer;
        struct StorageBuffer;
    }
}

namespace glint::engine::graphics {
    struct LayerRenderInfo {
        VkCommandBuffer commands;

        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        VkDescriptorSet cameraSet;
        VkDescriptorSet entitySet;
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