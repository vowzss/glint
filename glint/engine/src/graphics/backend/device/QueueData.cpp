#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"

namespace glint::engine::graphics {

    QueueData::QueueData(const VkDevice& device, const QueueFamilySupportDetails& family) {
        handles.resize(family.count);

        for (uint32_t i = 0; i < family.count; i++) {
            vkGetDeviceQueue(device, family.index, i, &handles[i]);
        }
    }

    QueuesData::QueuesData(const VkDevice& device, const QueueFamiliesSupportDetails& families)
        : graphics(QueueData{device, families.graphics}), present(QueueData{device, families.present}), compute(QueueData{device, families.compute}),
          transfer(QueueData{device, families.transfer}), sparse(QueueData{device, families.sparseBinding}) {
    }

}