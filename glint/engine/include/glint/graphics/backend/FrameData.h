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

            VkDescriptorPool descriptorPool;
            VkDescriptorSetLayout cameraDescriptorLayout;

          public:
            FrameDataInfo() = default;
        };

        struct FrameData {
            VkDevice device = {};
            VkDescriptorSet cameraDescriptorSet = {};

            std::unique_ptr<BufferData> buffer;

            VkSemaphore imageAvailable = {};
            VkSemaphore renderFinished = {};
            VkFence inFlight = {};

          public:
            FrameData() = delete;
            FrameData(const DeviceContext& devices, const FrameDataInfo& info);

            ~FrameData();

            void update(const scene::components::Camera& camera) const;
        };
    }
}