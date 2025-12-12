#include <vulkan/vulkan_core.h>

struct GLFWwindow;

namespace glint::engine {
    namespace graphics {
        class Renderer;
        struct Devices;
    }
}

namespace glint::engine::graphics {

    /*
    struct InterfaceLayerInfo {
        GLFWwindow* window;

        VkInstance instance;
        VkQueue queue;
        uint32_t queueFamily;
        VkRenderPass renderPass;
        VkCommandPool pool;
        uint32_t imageCount;
    };

    class InterfaceLayer : public RenderLayer {
        const VkDevice m_device = nullptr;
        VkDescriptorPool m_descriptorPool = nullptr;

      public:
        InterfaceLayer(const Devices& devices, const InterfaceLayerInfo& info);
        ~InterfaceLayer();

        void begin() override;
        void render(float dt, const LayerRenderInfo& info) override;
    };
    */

}