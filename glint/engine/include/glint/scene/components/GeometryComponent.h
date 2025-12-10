#pragma once

#include "glint/core/ecs/GeometryHandle.h"

namespace glint::engine::scene {

    struct GeometryComponent {
      public:
        core::GeometryHandle handle = core::GeometryHandle::invalid();
    };

}