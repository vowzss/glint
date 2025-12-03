#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace scene::components {
        struct Camera;
    }

    namespace graphics {
        struct Renderer;

        namespace backend {
            struct DeviceContext;
            struct BufferData;
        }

        namespace layers {
            struct SceneLayerInfo {
                VkPipeline pipeline;
                VkPipelineLayout pipelineLayout;

                const scene::components::Camera* camera;
            };

            struct SceneLayer : public RenderLayer {
              private:
                VkDevice device = {};
                SceneLayerInfo info;

                backend::BufferData* triangleVertexBuffer;
                backend::BufferData* triangleIndexBuffer;

                backend::BufferData* cubeVertexBuffer;
                backend::BufferData* cubeIndexBuffer;

              public:
                SceneLayer(const backend::DeviceContext& devices, SceneLayerInfo info_);
                ~SceneLayer();

                void renderFrame(const backend::FrameData& frame, VkCommandBuffer commands) override;
            };
        }
    }
}