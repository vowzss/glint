#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct QueueFamilySupportDetails;

    struct CommandsPoolObject {
        const VkDevice& m_device = nullptr;

        VkCommandPool m_handle = nullptr;
        std::vector<VkCommandBuffer> m_buffers;

      public:
        CommandsPoolObject() = delete;
        ~CommandsPoolObject();

        CommandsPoolObject(const CommandsPoolObject&) = delete;
        CommandsPoolObject& operator=(const CommandsPoolObject&) = delete;

        CommandsPoolObject(CommandsPoolObject&&) noexcept = delete;
        CommandsPoolObject& operator=(CommandsPoolObject&&) noexcept = delete;

        CommandsPoolObject(const VkDevice& device, const QueueFamilySupportDetails& family);

        // --- methods ---
        void begin(size_t idx);
        void end(size_t idx);

        VkCommandBuffer beginSingle() const;
        void endSingle(VkCommandBuffer commands, VkQueue queue) const;
    };
}