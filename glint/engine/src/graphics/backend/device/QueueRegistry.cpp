#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"
#include "glint/graphics/backend/device/QueueRegistry.h"

namespace glint::engine::graphics {

    QueueRegistry::QueueRegistry(const VkDevice& device, const QueueFamiliesSupportDetails& families)
        : graphics(QueueObject{device, families.graphics}), present(QueueObject{device, families.present}),
          compute(QueueObject{device, families.compute}), transfer(QueueObject{device, families.transfer}),
          sparse(QueueObject{device, families.sparseBinding}) {
    }

}