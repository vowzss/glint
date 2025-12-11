#pragma once

#include "BufferObject.h"

namespace glint::engine::graphics {

    struct UniformBufferObject : BufferObject {
        UniformBufferObject() = delete;
        UniformBufferObject(const Devices& devices, VkDeviceSize size, const void* data = nullptr)
            : BufferObject(devices, makeBufferInfo(size, data)) {};

      private:
        static BufferInfo makeBufferInfo(VkDeviceSize size, const void* data) {
            BufferInfo info = {};
            info.data = data;
            info.size = size;
            info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            return info;
        }
    };

}
