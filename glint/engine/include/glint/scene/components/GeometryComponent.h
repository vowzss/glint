#pragma once

#include "glint/core/ecs/GeometryHandle.h"

namespace glint::engine::scene::components {

    struct GeometryComponent {
      public:
        core::GeometryHandle handle = core::GeometryHandle::invalid();
    };

}