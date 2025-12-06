#pragma once

#include <memory>
#include <vector>

#include "glint/graphics/RendererContext.h"
#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/ImageBufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/renderpass/RenderpassData.h"
#include "glint/graphics/backend/swapchain/SwapchainData.h"
#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace scene::components {
        struct Camera;
    }

    namespace graphics {
        namespace backend {
            struct BufferData;
        }
    }
}

namespace glint::engine::graphics {

    class Renderer {
      private:
        int width;
        int height;
        RendererContext context;

        VkInstance instance;
        VkSurfaceKHR surface;

        backend::DeviceContext devices;
        VkDescriptorPool descriptorPool = {};

        std::unique_ptr<backend::QueuesData> queues;
        std::unique_ptr<backend::SwapchainData> swapchain;
        std::unique_ptr<backend::RenderpassData> renderpass;
        std::unique_ptr<backend::CommandsPoolData> commands;

        VkDescriptorSetLayout cameraDescriptorLayout = {};
        std::unique_ptr<scene::components::Camera> camera;

        VkDescriptorSetLayout entityDescriptorLayout = {};

        std::unique_ptr<backend::ImageBufferData> depthData;

        VkViewport viewport = {};
        VkRect2D scissor = {};

        VkPipelineLayout pipelineLayout; // layout for shaders (uniforms, descriptors)
        VkPipeline pipeline;             // encapsulates all GPU states and shaders

        int frameIndex = 0;
        uint32_t imageIndex = 0;
        std::vector<std::unique_ptr<backend::FrameData>> frames;

      public:
        Renderer() = delete;
        Renderer(int width_, int height_, const std::vector<const char*>& extensions_);

        ~Renderer();

        void init(const VkSurfaceKHR& surface_);

        void beginFrame();
        void recordFrame(float deltaTime);
        void endFrame();

        inline const VkInstance& getInstance() const { return instance; }
        inline scene::components::Camera& getCamera() const { return *camera; }

      private:
        // --- setup ---
        void createInstance();
        void createLogicalDevice();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();

        void createCamera();
    };

}