#pragma once

#include <cstdint>
#include <vector>

namespace glint::engine::graphics {

    struct QueueFamilyDetails {
        uint32_t index = UINT32_MAX;
        uint32_t count = 0;
        std::vector<float> priorities;

      public:
        inline bool available() const noexcept {
            return index != UINT32_MAX && count > 0;
        }
    };

    struct QueueFamiliesDetails {
        QueueFamilyDetails graphics;
        QueueFamilyDetails present;
        QueueFamilyDetails compute;
        QueueFamilyDetails transfer;
        QueueFamilyDetails sparseBinding;

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

        struct iterator {
            using ptr_type = QueueFamilyDetails*;
            ptr_type ptrs[5];
            size_t idx = 0;

            iterator(ptr_type* begin) {
                for (size_t i = 0; i < 5; ++i) {
                    ptrs[i] = begin[i];
                }
                idx = 0;
                advance_to_available();
            }

            void advance_to_available() {
                while (idx < 5 && !ptrs[idx]->available())
                    ++idx;
            }

            bool operator!=(const iterator& other) const {
                return idx != other.idx;
            }

            ptr_type operator*() const {
                return ptrs[idx];
            }

            iterator& operator++() {
                ++idx;
                advance_to_available();
                return *this;
            }
        };

        iterator begin() {
            QueueFamilyDetails* arr[5] = {&graphics, &present, &compute, &transfer, &sparseBinding};
            return iterator(arr);
        }

        iterator end() {
            QueueFamilyDetails* arr[5] = {&graphics, &present, &compute, &transfer, &sparseBinding};
            iterator it(arr);
            it.idx = 5;
            return it;
        }
    };

}