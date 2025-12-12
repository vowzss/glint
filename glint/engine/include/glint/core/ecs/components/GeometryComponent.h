#pragma once

#include "glint/core/ecs/ObjectHandle.h"

#include "Component.h"

namespace glint::engine::core {

    struct GeometryComponent : Component<core::GeometryHandle> {
        using Component::Component;
    };

}