#include "glint/scene/entity.h"

namespace glint::engine::scene {
    mat4 entity::getModelMatrix() const {
        return mat4::translate(transform.position) * mat4::rotate(transform.rotation) * mat4::scale(transform.scale);
    }
}