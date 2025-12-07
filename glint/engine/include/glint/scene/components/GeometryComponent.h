#pragma once

#include "glint/core/ecs/GeometryHandle.h"

namespace glint::engine::scene::components {

    struct GeometryComponent {
      private:
        using GeometryHandle = core::GeometryHandle;

      public:
        GeometryHandle handle;
    };

}