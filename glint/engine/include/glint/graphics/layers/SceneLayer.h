#pragma once

#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine {
    namespace core {
        class World;
    }
}

namespace glint::engine::graphics {
    struct StorageBuffer;

    class SceneLayer : public RenderLayer {
        const core::World& m_world;

      public:
        SceneLayer(const core::World& world);
        ~SceneLayer();

        void render(float dt, FrameObject& frame) override;
    };

}