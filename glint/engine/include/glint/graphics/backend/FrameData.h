#pragma once

#include <memory>
#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

#include "buffer/BufferData.h"

namespace glint::engine {
    namespace graphics::layers {
        struct RenderLayer;
    }
    namespace scene::components {
        struct CameraSnapshot;
    }
}

namespace glint::engine::graphics::backend {

    struct DeviceHandles;

    struct FrameCreateInfo {
        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout cameraLayout;
        VkDescriptorSetLayout entityLayout;

        const scene::components::CameraSnapshot& camera;
    };

    struct FrameRenderInfo {
        VkCommandBuffer commands = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        VkDescriptorSetLayout cameraLayout = nullptr;
        VkDescriptorSetLayout entityLayout = nullptr;

        const scene::components::CameraSnapshot& camera;
    };

    struct FrameData {
        VkDevice device = nullptr;

        VkDescriptorSet cameraSet = nullptr;
        VkDescriptorSet entitySet = nullptr;

        std::unique_ptr<BufferData> cameraBuffer;

        std::vector<layers::RenderLayer*> layers;

        // --- synchronisation ---
        VkSemaphore imageAvailable = nullptr;
        VkSemaphore renderFinished = nullptr;
        VkFence inFlight = nullptr;

        // --- timing ---
        mutable float deltaTime;

      public:
        FrameData() = delete;
        ~FrameData();

        FrameData(const DeviceHandles& devices, const FrameCreateInfo& info);

      public:
        void begin() const;
        void render(float deltaTime, const FrameRenderInfo& info) const;
        void end() const;

        void attach(layers::RenderLayer* layer);
        void detach(layers::RenderLayer* layer);
    };

}