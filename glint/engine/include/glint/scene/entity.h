#include "components/transform.h"
#include "glint/core/math/matrix/mat4.h"

struct mesh;

namespace glint::engine::scene {
    using mat4 = glint::engine::core::math::matrix::mat4;

    struct entity {
        uint32_t id;

        mesh* mesh = nullptr;
        components::transform transform{};

      public:
        mat4 getModelMatrix() const;
    };
}