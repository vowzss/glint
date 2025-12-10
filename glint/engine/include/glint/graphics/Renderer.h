#pragma once

#include <memory>
#include <vector>

#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/buffer/ImageBufferData.h"
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
        struct RenderLayer;
    }
}

namespace glint::engine::graphics {

    class Renderer {
        // --- window / viewport ---
        int width;
        int height;
        VkViewport viewport;
        VkRect2D scissor;

        // --- core ---
        VkInstance instance;
        VkSurfaceKHR surface;
        Devices devices;

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
        int frameIndex = 0;
        uint32_t imageIndex = 0;
        std::vector<std::unique_ptr<FrameData>> frames;

        // --- pipeline ---
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        std::unique_ptr<ImageBufferData> depthBuffer;

        // --- scene ---
        std::vector<std::unique_ptr<RenderLayer>> layers;

      public:
        Renderer() = delete;
        Renderer(int width_, int height_, const std::vector<const char*>& extensions);

        ~Renderer();

        void init(const VkSurfaceKHR& surface_);

        inline void append(std::unique_ptr<RenderLayer> layer) noexcept {
            layers.emplace_back(std::move(layer));
        }

        void begin() noexcept;
        void record(float deltaTime, const core::CameraSnapshot& snapshot) noexcept;
        void end() noexcept;

        // --- getters ---
        inline const VkInstance& getInstance() const noexcept {
            return instance;
        }

      private:
        void createInstance(const std::vector<const char*>& extensions);
        void createLogicalDevice();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();

        void createCameraLayout();
        void createEntityLayout();
    };

}