#include <vector>

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace scene {
        struct Entity;

        namespace components {
            struct Camera;
        }
    }

    namespace graphics {
        struct Renderer;

        namespace models {
            struct Mesh;
        }

        namespace backend {
            struct DeviceContext;
            struct BufferData;
        }

        namespace layers {
            struct SceneLayerCreateInfo {
                VkPipeline pipeline;
                VkPipelineLayout pipelineLayout;

                const scene::components::Camera& camera;
            };

            struct SceneLayer : public RenderLayer {
              private:
                VkDevice device = nullptr;
                SceneLayerCreateInfo info;

                std::vector<scene::Entity*> entities;
                backend::BufferData* entityBuffer;

                backend::BufferData* triangleVertexBuffer;
                backend::BufferData* triangleIndexBuffer;

                backend::BufferData* cubeVertexBuffer;
                backend::BufferData* cubeIndexBuffer;

              public:
                SceneLayer(const backend::DeviceContext& devices, SceneLayerCreateInfo info_);
                ~SceneLayer();

                void renderFrame(const backend::FrameData& frame, VkCommandBuffer commands) override;
            };
        }
    }
}