#pragma once

#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <vulkan/vulkan_core.h>

#include "glint/graphics/backend/buffer/StorageBuffer.h"
#include "glint/graphics/backend/buffer/UniformBuffer.h"
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

        const core::CameraSnapshot& camera;
    };

    template <typename T>
    struct FrameBufferBinding {
        VkDescriptorSet set;
        T buffer;
    };

    struct FrameData {
        const VkDevice m_device;

        FrameBufferBinding<UniformBuffer> m_camera;
        FrameBufferBinding<StorageBuffer> m_entity;

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

        void begin();
        void render(float deltaTime, const FrameRenderInfo& info);
        void end();

        void attach(RenderLayer* layer) noexcept;
        void detach(RenderLayer* layer) noexcept;
    };

}