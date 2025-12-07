#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

struct GLFWwindow;

namespace glint::engine {
    namespace graphics {
        struct Renderer;
        namespace backend {
            struct DeviceHandles;
        }
    }
}

namespace glint::engine::graphics::layers {

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
        VkDevice device = nullptr;
        VkDescriptorPool descriptorPool = nullptr;

      public:
        InterfaceLayer(const backend::DeviceHandles& devices, const InterfaceLayerInfo& info);
        ~InterfaceLayer();

        void begin() override;
        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}