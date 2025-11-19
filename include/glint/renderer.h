#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "glint/models/config/device_context.h"
#include "glint/models/config/renderer_info.h"
#include "glint/models/config/resolution_info.h"
#include "glint/models/data/commands_pool_data.h"
#include "glint/models/data/image_buffer_data.h"
#include "glint/models/data/queue_data.h"
#include "glint/models/data/renderpass_data.h"
#include "glint/models/data/swapchain_data.h"

#ifdef NDEBUG
const bool validationLayersEnabled = false;
#else
const bool validationLayersEnabled = true;
#endif

struct GLFWwindow;
struct buffer_data;

namespace glint {
    class renderer {
      private:
        const resolution_info res_;
        const renderer_info info_;

        VkInstance instance_;
        VkSurfaceKHR surface_;

        device_context devices_;

        std::unique_ptr<queues_data> queues_;
        std::unique_ptr<swapchain_data> swapchain_;
        std::unique_ptr<renderpass_data> renderpass_;
        std::unique_ptr<commands_pool_data> commands_;

        buffer_data* vertexBuffer_;
        std::unique_ptr<image_buffer_data> depthData_;

        VkViewport viewport{.x = 0.0f, .y = 0.0f, .minDepth = 0.0f, .maxDepth = 1.0f};
        VkRect2D scissor{.offset = {0, 0}};

        VkPipelineLayout pipelineLayout_; // layout for shaders (uniforms, descriptors)
        VkPipeline graphicsPipeline_;     // encapsulates all GPU states and shaders

        int frame_ = 0;
        std::vector<VkSemaphore> imageAvailableSemaphores_; // signals when a swapchain image is ready to render
        std::vector<VkSemaphore> renderFinishedSemaphores_; // signals when rendering is finished and ready to present
        std::vector<VkFence> inFlightFences_;               // fences to ensure CPU waits until GPU has finished work

      public:
        renderer() = delete;
        renderer(const resolution_info& res, const renderer_info& info);

        ~renderer();

        void init(GLFWwindow* window);
        void draw();

      private:
        void record(const buffer_data& buffer, uint32_t index);

        // --- setup ---
        void createInstance();
        void createSurface(GLFWwindow* window);
        void createLogicalDevice();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();
    };
}