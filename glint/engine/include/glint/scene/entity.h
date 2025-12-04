#pragma once

#include "components/Transform.h"

namespace glint::engine {
    namespace graphics::models {
        struct Mesh;
    }

    namespace scene {
        struct Entity {
            float elapsedTime = 0.0f;

          private:
            uint32_t id;

            components::Transform transform;

            mutable JPH::Mat44 model;
            mutable bool isModelDirty = true;

            graphics::models::Mesh* mesh;

            // todo: turn back to private
          public:
            Entity(uint32_t id_) : id(id_), mesh(nullptr) {
                transform.onChanged = [this]() { isModelDirty = true; };
            }

            Entity(uint32_t id_, graphics::models::Mesh* mesh_) : id(id_), mesh(mesh_) {
                transform.onChanged = [this]() { isModelDirty = true; };
            }

          public:
            inline uint32_t getId() const { return id; }
            inline graphics::models::Mesh& getMesh() const { return *mesh; }

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