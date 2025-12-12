#include "glint/graphics/backend/device/QueueFamilyDetails.h"
#include "glint/graphics/backend/device/QueueRegistry.h"

namespace glint::engine::graphics {

    QueueRegistry::QueueRegistry(const VkDevice& device, const QueueFamiliesDetails& families, uint32_t frames)
        : graphics(QueueObject{device, families.graphics, frames}), present(QueueObject{device, families.present, frames}),
          compute(QueueObject{device, families.compute, frames}) {

        if (families.transferAvailable()) {
            transfer.emplace(device, families.transfer, frames);
        }

        if (families.sparseAvailable()) {
            sparse.emplace(device, families.sparseBinding, frames);
        }
    }

}