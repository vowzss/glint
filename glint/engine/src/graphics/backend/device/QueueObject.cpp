#include <cstddef>
#include <cstdint>

#include "glint/graphics/backend/device/QueueFamilyDetails.h"
#include "glint/graphics/backend/device/QueueObject.h"

namespace glint::engine::graphics {

    QueueObject::QueueObject(const VkDevice& device, const QueueFamilyDetails& family, uint32_t frames) : m_pool(device, family.index, frames) {
#if defined(PLATFORM_MACOS)
        VkQueue queue;
        vkGetDeviceQueue(device, family.index, 0, &queue);
        m_handles.push_back(queue);
#else
        m_handles.resize(family.count);
        for (uint32_t i = 0; i < family.count; ++i) {
            vkGetDeviceQueue(device, family.index, i, &m_handles[i]);
        }
#endif
    }

}