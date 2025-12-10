#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace glint::engine::graphics {

    struct QueueFamilySupportDetails {
        uint32_t index = UINT32_MAX;
        uint32_t count = 0;
        std::vector<float> priorities;

      public:
        inline bool available() const noexcept {
            return index != UINT32_MAX && count > 0;
        }
    };

    struct QueueFamiliesSupportDetails {
        QueueFamilySupportDetails graphics;
        QueueFamilySupportDetails present;
        QueueFamilySupportDetails compute;
        QueueFamilySupportDetails transfer;
        QueueFamilySupportDetails sparseBinding;

      public:
        inline bool available() const noexcept {
            return graphicsAvailable() && presentAvailable();
        }

        // --- utility ---
        inline bool graphicsAvailable() const noexcept {
            return graphics.available();
        }
        inline bool presentAvailable() const noexcept {
            return present.available();
        }
        inline bool computeAvailable() const noexcept {
            return compute.available();
        }
        inline bool transferAvailable() const noexcept {
            return transfer.available();
        }
        inline bool sparseAvailable() const noexcept {
            return sparseBinding.available();
        }

        inline std::vector<const QueueFamilySupportDetails*> collect() const {
            std::vector<const QueueFamilySupportDetails*> unique;
            std::unordered_set<uint32_t> indices;

            for (const auto* family : {&graphics, &present, &compute, &transfer, &sparseBinding}) {
                if (!family->available()) {
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