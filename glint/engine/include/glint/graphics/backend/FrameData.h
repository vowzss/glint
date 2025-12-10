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
        VkCommandBuffer commands = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        VkDescriptorSetLayout cameraLayout = nullptr;
        VkDescriptorSetLayout entityLayout = nullptr;

        const core::CameraSnapshot& camera;
    };

    struct FrameData {
        const VkDevice m_device = nullptr;

        VkDescriptorSet m_cameraSet = nullptr;
        VkDescriptorSet m_entitySet = nullptr;

        std::unique_ptr<UniformBuffer> m_cameraBuffer;
        std::unique_ptr<StorageBuffer> m_entityBuffer;

        std::vector<RenderLayer*> m_layers;

        // --- synchronisation ---
        VkSemaphore imageAvailable = nullptr;
        VkSemaphore renderFinished = nullptr;
        VkFence inFlight = nullptr;

        // --- timing ---
        mutable float m_deltaTime;

      public:
        FrameData() = delete;
        FrameData(const Devices& devices, const FrameCreateInfo& info);

        ~FrameData();

        void begin() const;
        void render(float deltaTime, const FrameRenderInfo& info) const;
        void end() const;

        inline void tick(float dt) const noexcept {
            m_deltaTime = dt;
        }

        void attach(RenderLayer* layer) noexcept;
        void detach(RenderLayer* layer) noexcept;
    };

}