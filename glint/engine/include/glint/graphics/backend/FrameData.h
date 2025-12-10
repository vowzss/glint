#pragma once

#include <memory>
#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
    }
    namespace graphics {
        struct UniformBuffer;
        struct StorageBuffer;
    }
}

namespace glint::engine::graphics {

    struct Devices;
    struct RenderLayer;

    struct FrameCreateInfo {
        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout cameraLayout;
        VkDescriptorSetLayout entityLayout;
    };

    struct FrameRenderInfo {
        VkCommandBuffer commands;

        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        VkDescriptorSetLayout cameraLayout;
        VkDescriptorSetLayout entityLayout;

        const core::CameraSnapshot& camera;
    };

    struct FrameData {
        const VkDevice m_device;

        VkDescriptorSet m_cameraSet;
        VkDescriptorSet m_entitySet;

        std::unique_ptr<UniformBuffer> m_cameraBuffer;
        std::unique_ptr<StorageBuffer> m_entityBuffer;

        std::vector<RenderLayer*> m_layers;

        // --- synchronisation ---
        VkSemaphore m_ready;
        VkSemaphore m_done;
        VkFence m_guard;

        // --- timing ---
        mutable float m_deltaTime;

      public:
        FrameData() = delete;
        FrameData(const Devices& devices, const FrameCreateInfo& info);

        ~FrameData();

        void begin() const;
        void render(float deltaTime, const FrameRenderInfo& info) const;
        void end() const;

        void attach(RenderLayer* layer) noexcept;
        void detach(RenderLayer* layer) noexcept;
    };

}