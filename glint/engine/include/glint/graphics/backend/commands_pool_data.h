#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {
    struct queue_family_support_details;
    struct queue_families_support_details;

    struct commands_pool_data {
        VkDevice device = VK_NULL_HANDLE;

        VkCommandPool value = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> buffers;

      public:
        commands_pool_data() = delete;
        commands_pool_data(const VkDevice& device, const queue_family_support_details& family, size_t size);

        ~commands_pool_data();

        void begin(size_t idx);
        void end(size_t idx);
    };

/*
struct commands_pools_data {
    commands_pool_data graphics;
    commands_pool_data present;
    commands_pool_data compute;
    commands_pool_data transfer;
    commands_pool_data sparse;

    commands_pools_data() = delete;
    commands_pools_data(const VkDevice& device, const queue_families_support_details& families);

    ~commands_pools_data();
};
*/}