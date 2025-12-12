#pragma once

#include <vector>

#include "glint/graphics/backend/buffer/StorageBufferObject.h"
#include "glint/graphics/backend/buffer/UniformBufferObject.h"
#include "glint/graphics/layers/RenderLayer.h"

#include "FrameBufferBinding.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
        class World;
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

    struct FrameRecordContext {
        VkCommandBuffer commands;

        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        const core::CameraSnapshot& camera;
    };

    class FrameObject {
        const VkDevice m_device = nullptr;

        FrameBufferBinding<UniformBufferObject> m_camera;
        FrameBufferBinding<StorageBufferObject> m_entity;

        std::vector<std::reference_wrapper<RenderLayer>> m_layers;

      public:
        // --- synchronisation ---
        VkSemaphore m_ready = nullptr;
        VkSemaphore m_done = nullptr;
        VkFence m_guard = nullptr;

      private:
        const FrameRecordContext* m_context = nullptr;

      public:
        FrameObject() = delete;
        FrameObject(const Devices& devices, const FrameInfo& info);

        ~FrameObject();

        void begin();
        void record(float dt, const FrameRecordContext& context);
        void end();

        void attach(RenderLayer& layer) noexcept;
        void detach(RenderLayer& layer) noexcept;

        // --- getters ---
        inline const FrameRecordContext& context() const {
            if (m_context == nullptr) {
                throw std::runtime_error("Frame context is no longer valid!");
            }

            return *m_context;
        }

        inline const FrameBufferBinding<UniformBufferObject>& camera() const noexcept {
            return m_camera;
        }

        inline FrameBufferBinding<UniformBufferObject>& camera() noexcept {
            return m_camera;
        }

        inline const FrameBufferBinding<StorageBufferObject>& entity() const noexcept {
            return m_entity;
        }

        inline FrameBufferBinding<StorageBufferObject>& entity() noexcept {
            return m_entity;
        }
    };

}