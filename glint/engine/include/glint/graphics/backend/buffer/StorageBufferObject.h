#pragma once

#include "BufferObject.h"

namespace glint::engine::graphics {

    struct StorageBufferObject : BufferObject {
        StorageBufferObject() = delete;
        StorageBufferObject(const Devices& devices, VkDeviceSize size, const void* data = nullptr)
            : BufferObject(devices, makeBufferInfo(size, data)) {};

      private:
        static BufferInfo makeBufferInfo(VkDeviceSize size, const void* data) {
            BufferInfo info = {};
            info.data = data;
            info.size = size;
            info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            return info;
        }
    };

}
