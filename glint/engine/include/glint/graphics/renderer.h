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

        class Renderer {
          private:
            int width;
            int height;
            RendererContext context;

            VkInstance instance_;
            VkSurfaceKHR surface_;

            backend::DeviceContext devices_;
            VkDescriptorPool descriptorPool_{};

            std::unique_ptr<backend::QueuesData> queues_;
            std::unique_ptr<backend::SwapchainData> swapchain_;
            std::unique_ptr<backend::RenderpassData> renderpass_;
            std::unique_ptr<backend::CommandsPoolData> commands_;

            VkDescriptorSetLayout cameraDescriptorLayout_{};
            std::unique_ptr<scene::components::Camera> camera_;

            std::vector<layers::RenderLayer*> renderLayers;

            std::unique_ptr<backend::ImageBufferData> depthData_;

            VkViewport viewport_{};
            VkRect2D scissor_{};

            VkPipelineLayout pipelineLayout_; // layout for shaders (uniforms, descriptors)
            VkPipeline graphicsPipeline_;     // encapsulates all GPU states and shaders

            int frameIndex_ = 0;
            uint32_t imageIndex_ = 0;
            std::vector<std::unique_ptr<backend::FrameData>> frames;

          public:
            Renderer() = delete;
            Renderer(int width_, int height_, const std::vector<const char*>& extensions_);

            ~Renderer();

            void init(VkSurfaceKHR&& surface);

            void beginFrame();
            void recordFrame();
            void endFrame();

            inline const VkInstance& getInstance() const { return instance_; }
            inline scene::components::Camera& getCamera() const { return *camera_; }

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
}