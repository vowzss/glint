#pragma once

namespace glint::engine::graphics {
    class FrameObject;

    class RenderLayer {
      public:
        virtual ~RenderLayer() = default;

        virtual void begin(FrameObject& frame) {};
        virtual void render(float dt, FrameObject& frame) = 0;
        virtual void end(FrameObject& frame) {};
    };

}