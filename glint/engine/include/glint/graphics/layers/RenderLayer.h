#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics {

    namespace backend {
        struct FrameData;
    }

    namespace layers {
        struct RenderLayer {
          public:
            virtual ~RenderLayer() = default;

            virtual void beginFrame() {};
            virtual void renderFrame(const backend::FrameData& frame, VkCommandBuffer commands) = 0;
            virtual void endFrame() {};
        };
    }
}