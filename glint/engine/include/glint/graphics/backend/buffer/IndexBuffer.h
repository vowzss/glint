#pragma once

#include "BufferData.h"

namespace glint::engine::graphics::backend {

    struct IndexBuffer : BufferData {
        IndexBuffer(const DeviceContext& devices, const void* data, VkDeviceSize size) {
            BufferCreateInfo createInfo = {};
            createInfo.data = data;
            createInfo.size = size;
            createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            createInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

            init(devices, createInfo);
        }
    };

}
