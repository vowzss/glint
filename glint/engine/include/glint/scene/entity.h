#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

#include "components/transform.h"

struct mesh;

namespace glint::engine::scene {
    struct entity {
        uint32_t id;

        mesh* mesh = nullptr;
        components::transform transform{};

      public:
        JPH::Mat44 getModelMatrix() const;
    };
}