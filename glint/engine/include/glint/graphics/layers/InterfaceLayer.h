#include <vulkan/vulkan_core.h>

#include "glint/graphics/layers/RenderLayer.h"

struct GLFWwindow;

namespace glint::engine {
    namespace graphics {
        struct Renderer;
        struct Devices;
    }
}

namespace glint::engine::graphics {

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
        InterfaceLayer(const Devices& devices, const InterfaceLayerInfo& info);
        ~InterfaceLayer();

        void begin() override;
        void render(float deltaTime, const LayerRenderInfo& info) override;
    };

}