#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"
#include "glint/graphics/backend/device/QueueObject.h"

namespace glint::engine::graphics {

    QueueObject::QueueObject(const VkDevice& device, const QueueFamilySupportDetails& family) : m_family(family.index), m_pool(device, family) {
        m_handles.reserve(family.count);
        for (uint32_t i = 0; i < family.count; i++) {
            vkGetDeviceQueue(device, family.index, i, &m_handles[i]);
        }
    }

}