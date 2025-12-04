#include "glint/graphics/layers/RenderLayer.h"

struct GLFWwindow;

namespace glint::engine::graphics {
    struct Renderer;

    namespace backend {
        struct DeviceContext;
    }

    namespace layers {
        struct InterfaceLayerInfo {
            GLFWwindow* window;

            VkInstance instance;
            VkQueue queue;
            uint32_t queueFamily;
            VkRenderPass renderPass;
            VkCommandPool pool;
            uint32_t imageCount;
        };

        struct InterfaceLayer : public RenderLayer {
          private:
            VkDevice device = {};
            VkDescriptorPool descriptorPool = {};

          public:
            InterfaceLayer(const backend::DeviceContext& devices, InterfaceLayerInfo info);
            ~InterfaceLayer();

            void beginFrame() override;
            void renderFrame(const backend::FrameData& frame, VkCommandBuffer commands) override;
        };
    }
}