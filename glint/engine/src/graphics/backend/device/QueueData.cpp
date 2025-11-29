#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"

namespace glint::engine::graphics::backend {
    QueueData::QueueData(const VkDevice& device, const QueueFamilySupportDetails& family) : values({}) {
        values.resize(family.count);

        for (uint32_t i = 0; i < family.count; i++) {
            vkGetDeviceQueue(device, family.index, i, &values[i]);
        }
    }

    QueuesData::QueuesData(const VkDevice& device, const QueueFamiliesSupportDetails& families)
        : graphics(QueueData{device, families.graphics}), present(QueueData{device, families.present}), compute(QueueData{device, families.compute}),
          transfer(QueueData{device, families.transfer}), sparse(QueueData{device, families.sparseBinding}) {
    }
}