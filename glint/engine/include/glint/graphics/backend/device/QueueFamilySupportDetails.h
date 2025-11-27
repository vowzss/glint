#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace glint::engine::graphics {

    namespace backend {
        struct QueueFamilySupportDetails {
          public:
            uint32_t index = UINT32_MAX;
            uint32_t count = 0;
            std::vector<float> priorities;

          public:
            QueueFamilySupportDetails() = default;

            inline bool isReady() const { return index != UINT32_MAX && count > 0; }
        };

        struct QueueFamiliesSupportDetails {
            QueueFamilySupportDetails graphics{};
            QueueFamilySupportDetails present{};
            QueueFamilySupportDetails compute{};
            QueueFamilySupportDetails transfer{};
            QueueFamilySupportDetails sparseBinding{};

          public:
            QueueFamiliesSupportDetails() = default;

            // --- utility ---
            inline bool isGraphicsReady() const { return graphics.isReady(); }
            inline bool isPresentReady() const { return present.isReady(); }
            inline bool isComputeReady() const { return compute.isReady(); }
            inline bool isTransferReady() const { return transfer.isReady(); }
            inline bool isSparseReady() const { return sparseBinding.isReady(); }

            // todo: for now only care about graphics + present queue
            inline bool isReady() const { return isGraphicsReady() && isPresentReady(); }

            inline std::vector<const QueueFamilySupportDetails*> collect() const {
                std::vector<const QueueFamilySupportDetails*> unique;
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
}