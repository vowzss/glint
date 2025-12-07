#pragma once

#include "glint/graphics/backend/device/DeviceHandles.h"

namespace glint::engine::graphics::layers {

    struct LayerRenderInfo {
        VkCommandBuffer commands = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        VkDescriptorSet cameraSet = nullptr;
        VkDescriptorSet entitySet = nullptr;
    };

    struct RenderLayer {
      protected:
        RenderLayer() = default;

      public:
        virtual ~RenderLayer() = default;

        virtual void init(const backend::DeviceHandles& devices) = 0;

        virtual void begin() {};
        virtual void render(float deltaTime, const LayerRenderInfo& info) = 0;
        virtual void end() {};
    };

}