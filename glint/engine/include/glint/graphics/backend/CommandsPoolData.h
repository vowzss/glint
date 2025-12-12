#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    class CommandsPoolObject {
        const VkDevice& m_device;

        VkCommandPool m_handle = nullptr;
        std::vector<VkCommandBuffer> m_buffers;

      public:
        CommandsPoolObject() = delete;
        ~CommandsPoolObject();

        CommandsPoolObject(const CommandsPoolObject&) = delete;
        CommandsPoolObject& operator=(const CommandsPoolObject&) = delete;

        CommandsPoolObject(CommandsPoolObject&&) noexcept = delete;
        CommandsPoolObject& operator=(CommandsPoolObject&&) noexcept = delete;

        CommandsPoolObject(const VkDevice& device, uint32_t family, uint32_t frames);

        // --- methods ---
        const VkCommandBuffer& begin(size_t idx);
        void end(size_t idx);

        VkCommandBuffer beginSingle() const;
        void endSingle(VkCommandBuffer commands, VkQueue queue) const;

        // --- getters ---
        inline const VkCommandPool& handle() const {
            return m_handle;
        }

        // --- operators ---
        inline const VkCommandBuffer& operator[](size_t frame) const noexcept {
            return m_buffers[frame];
        }
    };
}