#pragma once

#include "BufferData.h"

namespace glint::engine::graphics {

    struct IndexBuffer : BufferData {
        IndexBuffer() = delete;
        IndexBuffer(const Devices& devices, VkDeviceSize size, const void* data = nullptr) : BufferData(devices, makeBufferInfo(size, data)) {};

      private:
        static BufferCreateInfo makeBufferInfo(VkDeviceSize size, const void* data) {
            BufferCreateInfo info = {};
            info.data = data;
            info.size = size;
            info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            return info;
        }
    };

}
