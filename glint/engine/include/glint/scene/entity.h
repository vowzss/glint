#pragma once

#include "components/Transform.h"

namespace glint::engine {
    namespace graphics::models {
        struct Mesh;
    }

    namespace scene {
        struct Entity {
          private:
            uint32_t id;

            components::Transform transform;

            mutable JPH::Mat44 model;
            mutable bool isModelDirty = true;

            graphics::models::Mesh* mesh;

          private:
            Entity(uint32_t id_) : id(id_), mesh(nullptr) {}

          public:
            inline uint32_t getId() const { return id; }

            inline components::Transform& getTransform() { return transform; }
            inline const JPH::Mat44& getModelMatrix() const {
                if (isModelDirty) {
                    model = transform.toMatrix();
                    isModelDirty = false;
                }

                return model;
            }
        };
    }
}