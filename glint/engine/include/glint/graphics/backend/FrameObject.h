#pragma once

#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <vulkan/vulkan_core.h>

#include "glint/graphics/backend/buffer/StorageBufferObject.h"
#include "glint/graphics/backend/buffer/UniformBufferObject.h"
#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
        class World;
    }
    namespace graphics {
        struct UniformBuffer;
        struct StorageBuffer;
    }
}

namespace glint::engine::graphics {

    struct Devices;
    class RenderLayer;

    struct FrameInfo {
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

    struct FrameObject {
        const VkDevice m_device = nullptr;

        FrameBufferBinding<UniformBufferObject> m_camera;
        FrameBufferBinding<StorageBufferObject> m_entity;

        std::vector<RenderLayer*> m_layers;

        // --- synchronisation ---
        VkSemaphore m_ready = nullptr;
        VkSemaphore m_done = nullptr;
        VkFence m_guard = nullptr;

        // --- timing ---
        mutable float m_deltaTime;

      public:
        FrameObject() = delete;
        FrameObject(const Devices& devices, const FrameInfo& info);

        ~FrameObject();

        void begin();
        void render(float deltaTime, const FrameRenderInfo& info);
        void end();

        void attach(RenderLayer* layer) noexcept;
        void detach(RenderLayer* layer) noexcept;
    };

}