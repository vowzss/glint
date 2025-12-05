#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "buffer/BufferData.h"

namespace glint::engine {
    namespace scene::components {
        struct Camera;
    }

    namespace graphics {
        namespace layers {
            struct RenderLayer;
        }

        namespace backend {
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

                std::vector<layers::RenderLayer*> layers;

                VkSemaphore imageAvailable = nullptr;
                VkSemaphore renderFinished = nullptr;
                VkFence inFlight = nullptr;

                mutable float deltaTime;

              public:
                FrameData() = delete;
                FrameData(const DeviceContext& devices, const FrameDataCreateInfo& info);

                ~FrameData();

                void begin() const;
                void render(float deltaTime, const scene::components::Camera& camera, const VkCommandBuffer& command) const;
                void end() const;
            };
        }
    }
}