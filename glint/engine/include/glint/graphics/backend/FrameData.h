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

        struct FrameDataCreateInfo {
            VkDescriptorPool descriptorPool;

            VkDescriptorSetLayout cameraDescriptorLayout;
            VkDescriptorSetLayout entityDescriptorLayout;

            const scene::components::Camera& camera;
        };

        struct FrameData {
            VkDevice device = nullptr;

            VkDescriptorSet cameraDescriptorSet = nullptr;
            std::unique_ptr<BufferData> cameraBuffer;

            VkDescriptorSet entityDescriptorSet = nullptr;

            VkSemaphore imageAvailable = nullptr;
            VkSemaphore renderFinished = nullptr;
            VkFence inFlight = nullptr;

            float deltaTime;

          public:
            FrameData() = delete;
            FrameData(const DeviceContext& devices, const FrameDataCreateInfo& info);

            ~FrameData();

            void update(const scene::components::Camera& camera) const;
        };
    }
}