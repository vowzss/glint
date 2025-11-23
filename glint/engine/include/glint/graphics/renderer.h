#pragma once

#include <memory>
#include <vector>

#include "glint/graphics/backend/buffer/image_buffer_data.h"
#include "glint/graphics/backend/commands_pool_data.h"
#include "glint/graphics/backend/device/device_context.h"
#include "glint/graphics/backend/device/queue_data.h"
#include "glint/graphics/backend/renderpass/renderpass_data.h"
#include "glint/graphics/backend/swapchain/swapchain_data.h"
#include "glint/graphics/renderer_info.h"

#ifdef NDEBUG
const bool validationLayersEnabled = false;
#else
const bool validationLayersEnabled = true;
#endif

struct GLFWwindow;

namespace glint::engine::graphics::backend {
    struct buffer_data;
}

namespace glint::engine::graphics {
    class renderer {
      private:
        int width_;
        int height_;
        const renderer_info info_;

        VkInstance instance_;
        VkSurfaceKHR surface_;

        backend::device_context devices_;

        std::unique_ptr<backend::queues_data> queues_;
        std::unique_ptr<backend::swapchain_data> swapchain_;
        std::unique_ptr<backend::renderpass_data> renderpass_;
        std::unique_ptr<backend::commands_pool_data> commands_;

        backend::buffer_data* vertexBuffer_;
        std::unique_ptr<backend::image_buffer_data> depthData_;

        VkViewport viewport_{};
        VkRect2D scissor_{};

        VkPipelineLayout pipelineLayout_; // layout for shaders (uniforms, descriptors)
        VkPipeline graphicsPipeline_;     // encapsulates all GPU states and shaders

        int frame_ = 0;
        std::vector<VkSemaphore> imageAvailableSemaphores_; // signals when a swapchain image is ready to render
        std::vector<VkSemaphore> renderFinishedSemaphores_; // signals when rendering is finished and ready to present
        std::vector<VkFence> inFlightFences_;               // fences to ensure CPU waits until GPU has finished work

      public:
        renderer() = delete;
        renderer(int w, int h, const renderer_info& info);

        ~renderer();

        void init(VkSurfaceKHR surface);
        void draw();

        inline const VkInstance& getInstance() const { return instance_; }

      private:
        void record(const backend::buffer_data& buffer, uint32_t index);

        // --- setup ---
        void createInstance();
        void createLogicalDevice();

        void createSwapchain();
        void createRenderPass();
        void createGraphicsPipeline();

        void createCommandPool();
        void createSyncObjects();
    };
}