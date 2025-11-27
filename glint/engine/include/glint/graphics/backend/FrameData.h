#pragma once

#include <memory>

#include <vulkan/vulkan_core.h>

#include "buffer/BufferData.h"

namespace glint::engine {
    namespace scene::components {
        struct Camera;
    }

    namespace graphics::backend {
        struct DeviceContext;

        struct FrameDataInfo {
            scene::components::Camera* camera;
            VkDescriptorPool pool;
            VkDescriptorSetLayout layout;

          public:
            FrameDataInfo() = default;
        };

        struct FrameData {
            VkDevice device = {};

            VkDescriptorSet cameraSet = {};
            std::unique_ptr<BufferData> cameraBuffer;

            VkSemaphore imageAvailable = {};
            VkSemaphore renderFinished = {};
            VkFence inFlight = {};

          public:
            FrameData() = delete;
            FrameData(const DeviceContext& devices, const FrameDataInfo& info);

            ~FrameData();
        };
    }
}