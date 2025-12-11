#pragma once

#include <memory>
#include <vector>

#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/buffer/ImageBufferObject.h"
#include "glint/graphics/backend/device/Devices.h"
#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/renderpass/RenderpassData.h"
#include "glint/graphics/backend/swapchain/SwapchainData.h"

namespace glint::engine {
    namespace core {
        struct CameraSnapshot;
    }
    namespace graphics {
        struct FrameData;
        class RenderLayer;
    }
}

namespace glint::engine::graphics {

    class Renderer {
        // --- window / viewport ---
        int m_width, m_height;
        VkViewport m_viewport;
        VkRect2D m_scissor;

        // --- core ---
        VkInstance m_instance;
        VkSurfaceKHR m_surface;
        Devices m_devices;

        // --- swapchain + renderpass ---
        std::unique_ptr<SwapchainData> swapchain;
        std::unique_ptr<RenderpassData> renderpass;
        std::unique_ptr<CommandsPoolData> commands;

        // --- queues ---
        std::unique_ptr<QueuesData> queues;

        // --- descriptors ---
        VkDescriptorPool descriptorPool = nullptr;
        VkDescriptorSetLayout cameraLayout = nullptr;
        VkDescriptorSetLayout entityLayout = nullptr;

        // --- frame ---
        int m_frame = 0;
        uint32_t m_image = 0;
        std::vector<std::unique_ptr<FrameData>> m_frames;

        // --- pipeline ---
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::unique_ptr<ImageBufferData> depthBuffer;

        // --- scene ---
        std::vector<std::unique_ptr<RenderLayer>> layers;

      public:
        Renderer() = delete;
        Renderer(int width, int height, const std::vector<const char*>& extensions);

        ~Renderer();

        void init(const VkSurfaceKHR& surface);

        inline void append(std::unique_ptr<RenderLayer> layer) noexcept {
            layers.emplace_back(std::move(layer));
        }

        void begin() noexcept;
        void record(float deltaTime, const core::CameraSnapshot& snapshot) noexcept;
        void end() noexcept;

        // --- getters ---
        inline const VkInstance& getInstance() const noexcept {
            return m_instance;
        }

        // todo: debug only for now
        inline const Devices& getDevices() const noexcept {
            return m_devices;
        }

      private:
        void createInstance(const std::vector<const char*>& extensions);
        void createDevices();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();

        void createCameraLayout();
        void createEntityLayout();
    };

}