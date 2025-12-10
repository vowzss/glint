#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct QueueFamilySupportDetails;

    struct CommandsPoolData {
        VkDevice device = {};

        VkCommandPool value = {};
        std::vector<VkCommandBuffer> buffers = {};

      public:
        CommandsPoolData() = delete;
        CommandsPoolData(const VkDevice& device, const QueueFamilySupportDetails& family, size_t size);

        ~CommandsPoolData();

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

    */
}