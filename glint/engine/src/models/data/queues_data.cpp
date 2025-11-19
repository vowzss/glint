
#include <vulkan/vulkan_core.h>

#include "glint/models/data/queue_data.h"
#include "glint/models/misc/queue_family_support_details.h"

queue_data::queue_data(const VkDevice& device, const queue_family_support_details& family)
    : values({}) {
    values.resize(family.count);

    for (uint32_t i = 0; i < family.count; i++) {
        vkGetDeviceQueue(device, family.index, i, &values[i]);
    }
}

queues_data::queues_data(const VkDevice& device, const queue_families_support_details& families)
    : graphics(queue_data{device, families.graphics}),
      present(queue_data{device, families.present}),
      compute(queue_data{device, families.compute}),
      transfer(queue_data{device, families.transfer}),
      sparse(queue_data{device, families.sparseBinding}) {}