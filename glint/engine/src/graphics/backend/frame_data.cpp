#include "glint/graphics/backend/frame_data.h"

#include "glint/core/math/matrix/mat4.h"
#include "glint/graphics/backend/buffer/buffer_data_info.h"
#include "glint/graphics/backend/device/device_context.h"
#include "glint/scene/components/camera.h"

namespace glint::engine::graphics::backend {
    frame_data::frame_data(const device_context& devices, const frame_data_info& info) : device(devices.logical) {
        using namespace glint::engine::core::math;

        VkSemaphoreCreateInfo semInfo{};
        semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(device, &semInfo, nullptr, &imageAvailable);
        vkCreateSemaphore(device, &semInfo, nullptr, &renderFinished);

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &fenceInfo, nullptr, &inFlight);

        matrix::mat4 viewMatrix = info.camera->getViewMatrix();
        matrix::mat4 projMatrix = info.camera->getProjectionMatrix();
        matrix::mat4 viewProjMatrix = viewMatrix * projMatrix;
        matrix::mat4 cameraMatrices[3] = {viewMatrix, projMatrix, viewProjMatrix};

        buffer_data_info dataInfo{};
        dataInfo.data = cameraMatrices;
        dataInfo.size = sizeof(matrix::mat4) * 3;
        dataInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        dataInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        cameraBuffer = std::make_unique<buffer_data>(devices, dataInfo);

        VkDescriptorSetAllocateInfo cameraAllocInfo{};
        cameraAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        cameraAllocInfo.descriptorPool = info.pool;
        cameraAllocInfo.descriptorSetCount = 1;
        cameraAllocInfo.pSetLayouts = &info.layout;

        if (vkAllocateDescriptorSets(device, &cameraAllocInfo, &cameraSet) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create camera descriptor!");
        }

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = cameraBuffer->value;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(matrix::mat4) * 3;

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.dstSet = cameraSet;
        writeInfo.dstBinding = 0;
        writeInfo.dstArrayElement = 0;
        writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeInfo.descriptorCount = 1;
        writeInfo.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    frame_data::~frame_data() {
        if (inFlight != VK_NULL_HANDLE) {
            vkDestroyFence(device, inFlight, nullptr);
        }

        if (imageAvailable != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, imageAvailable, nullptr);
        }

        if (renderFinished != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, renderFinished, nullptr);
        }
    }
}