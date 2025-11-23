#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace glint::engine::graphics::backend {
    struct queue_family_support_details {
        uint32_t index = UINT32_MAX;
        uint32_t count = 0;
        std::vector<float> priorities;

      public:
        queue_family_support_details() = default;

        // --- utility ---
        inline bool isReady() const { return index != UINT32_MAX && count > 0; }
    };

    struct queue_families_support_details {
        queue_family_support_details graphics{};
        queue_family_support_details present{};
        queue_family_support_details compute{};
        queue_family_support_details transfer{};
        queue_family_support_details sparseBinding{};

      public:
        queue_families_support_details() = default;

        // --- utility ---
        inline bool isGraphicsReady() const { return graphics.isReady(); }
        inline bool isPresentReady() const { return present.isReady(); }
        inline bool isComputeReady() const { return compute.isReady(); }
        inline bool isTransferReady() const { return transfer.isReady(); }
        inline bool isSparseReady() const { return sparseBinding.isReady(); }

        // todo: for now only care about graphics + present queue
        inline bool isReady() const { return isGraphicsReady() && isPresentReady(); }

        inline std::vector<const queue_family_support_details*> collect() const {
            std::vector<const queue_family_support_details*> unique;
            std::unordered_set<uint32_t> indices;

            for (const auto* family : {&graphics, &present, &compute, &transfer, &sparseBinding}) {
                if (!family->isReady()) {
                    continue;
                }

                if (indices.insert(family->index).second) {
                    unique.push_back(family);
                }
            }

            return unique;
        }
    };
}