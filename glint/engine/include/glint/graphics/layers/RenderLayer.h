#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine {
    namespace graphics {
        struct UniformBuffer;
        struct StorageBuffer;
    }
}

namespace glint::engine::graphics {

    template <typename T>
    struct BufferRenderInfo {
        VkDescriptorSet set;
        T* data;
    };

    struct LayerRenderInfo {
        VkCommandBuffer commands;

        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        BufferRenderInfo<UniformBuffer> camera;
        BufferRenderInfo<StorageBuffer> entity;
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