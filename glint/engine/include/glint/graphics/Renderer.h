#pragma once

#include <memory>
#include <vector>

#include "glint/graphics/RendererContext.h"
#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/buffer/ImageBufferData.h"
#include "glint/graphics/backend/device/DeviceHandles.h"
#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/renderpass/RenderpassData.h"
#include "glint/graphics/backend/swapchain/SwapchainData.h"

namespace glint::engine {
    namespace core {
        struct Registry;
    }
    namespace scene {
        struct World;
        namespace components {
            struct Camera;
        }
    }

    namespace graphics {
        namespace backend {
            struct FrameData;
            struct BufferData;
        }
        namespace layers {
            struct RenderLayer;
        }
    }
}

namespace glint::engine::graphics {

    class Renderer {
      private:
        // --- window / viewport ---
        int width;
        int height;
        VkViewport viewport;
        VkRect2D scissor;

        // --- core ---
        VkInstance instance;
        VkSurfaceKHR surface;
        backend::DeviceHandles devices;

        // --- swapchain + renderpass ---
        std::unique_ptr<backend::SwapchainData> swapchain;
        std::unique_ptr<backend::RenderpassData> renderpass;
        std::unique_ptr<backend::CommandsPoolData> commands;

        // --- queues ---
        std::unique_ptr<backend::QueuesData> queues;

        // --- descriptors ---
        VkDescriptorPool descriptorPool = nullptr;
        VkDescriptorSetLayout cameraLayout = nullptr;
        VkDescriptorSetLayout entityLayout = nullptr;

        // --- frame ---
        int frameIndex = 0;
        uint32_t imageIndex = 0;
        std::vector<std::unique_ptr<backend::FrameData>> frames;

        // --- scene ---
        std::vector<std::unique_ptr<layers::RenderLayer>> layers;
        std::unique_ptr<scene::components::Camera> camera;

        // --- pipeline ---
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        std::unique_ptr<backend::ImageBufferData> depthBuffer;

      public:
        Renderer() = delete;
        ~Renderer();

        Renderer(int width_, int height_, const std::vector<const char*>& extensions_);

      public:
        void init(const VkSurfaceKHR& surface_);

        inline void append(std::unique_ptr<layers::RenderLayer> layer) {
            layers.emplace_back(std::move(layer));
        }

        void beginFrame();
        void recordFrame(float deltaTime);
        void endFrame();

        // --- getters ---
        inline const VkInstance& getInstance() const {
            return instance;
        }
        inline scene::components::Camera& getCamera() const {
            return *camera;
        }

      private:
        void createInstance(const RendererContext& context);
        void createLogicalDevice();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();

        void createCamera();
    };

}