#include "glint/components/transform.h"
#include "glint/core/math/matrix/mat4.h"

struct mesh;

struct entity {
    uint32_t id;

    mesh* mesh = nullptr;
    transform transform{};

    mat4 get_model_matrix() const { return mat4::translate(transform.position) * mat4::rotate(transform.rotation) * mat4::scale(transform.scale); }
};
