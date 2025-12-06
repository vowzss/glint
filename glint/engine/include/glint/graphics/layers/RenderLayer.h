#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics::backend {
    struct FrameData;
}

namespace glint::engine::graphics::layers {

    struct RenderLayer {
      public:
        virtual ~RenderLayer() = default;

        virtual void begin(){};
        virtual void render(float deltaTime, const VkCommandBuffer& commands) = 0;
        virtual void end(){};
    };

}