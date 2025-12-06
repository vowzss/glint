#pragma once

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        struct EcsManager;
    }

    namespace graphics {
        namespace models {
            struct Mesh;
        }

        namespace backend {
            struct DeviceContext;
            struct BufferData;
        }
    }

    namespace scene {
        namespace components {
            struct Camera;
        }
    }
}

namespace glint::engine::graphics::layers {

    struct SceneLayerCreateInfo {
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;

        const scene::components::Camera& camera;
        const core::EcsManager& ecs;
    };

    struct SceneLayer : public RenderLayer {
      private:
        VkDevice device = nullptr;
        SceneLayerCreateInfo info;

        backend::BufferData* entityBuffer;

      public:
        SceneLayer(const backend::DeviceContext& devices, SceneLayerCreateInfo info_);
        ~SceneLayer() = default;

        void render(float deltaTime, const VkCommandBuffer& commands) override;
    };

}