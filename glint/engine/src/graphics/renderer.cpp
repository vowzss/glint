#include "glint/graphics/renderer.h"

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "glint/misc/vk_helpers.h"
#include "glint/models/data/buffer_data.h"
#include "glint/models/data/queue_data.h"
#include "glint/models/data/renderpass_data.h"
#include "glint/models/data/swapchain_data.h"
#include "glint/models/misc/buffer_data_info.h"
#include "glint/models/misc/image_buffer_data_info.h"
#include "glint/models/misc/queue_family_support_details.h"
#include "glint/models/misc/renderpass_attachment_info.h"
#include "glint/models/objects/vertex.h"
#include "glint/utils/file_utils.h"
#include "glint/utils/vk_utils.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace glint::engine::graphics {
    renderer::renderer(int w, int h, const renderer_info& info) : width_(w), height_(h), info_(info) {
        createInstance();
    }

    renderer::~renderer() {
        vkDeviceWaitIdle(devices_.logical);

        for (VkFence fence : inFlightFences_) {
            vkDestroyFence(devices_.logical, fence, nullptr);
        }

        for (VkSemaphore sem : imageAvailableSemaphores_) {
            vkDestroySemaphore(devices_.logical, sem, nullptr);
        }

        for (VkSemaphore sem : renderFinishedSemaphores_) {
            vkDestroySemaphore(devices_.logical, sem, nullptr);
        }

        for (VkFramebuffer fb : renderpass_->framebuffers)
            vkDestroyFramebuffer(devices_.logical, fb, nullptr);

        renderpass_->framebuffers.clear();

        vkDestroyImageView(devices_.logical, depthData_->view, nullptr);
        for (VkImageView view : swapchain_->views)
            vkDestroyImageView(devices_.logical, view, nullptr);

        vkDestroySwapchainKHR(devices_.logical, swapchain_->value, nullptr);

        vkDestroyImage(devices_.logical, depthData_->image, nullptr);
        vkFreeMemory(devices_.logical, depthData_->memory, nullptr);

        vkDestroyBuffer(devices_.logical, vertexBuffer_->value, nullptr);
        vkFreeMemory(devices_.logical, vertexBuffer_->memory, nullptr);

        vkDestroyPipeline(devices_.logical, graphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(devices_.logical, pipelineLayout_, nullptr);
        vkDestroyRenderPass(devices_.logical, renderpass_->value, nullptr);

        vkFreeCommandBuffers(devices_.logical, commands_->value, commands_->buffers.size(), commands_->buffers.data());
        vkDestroyCommandPool(devices_.logical, commands_->value, nullptr);

        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        vkDestroyDevice(devices_.logical, nullptr);
        vkDestroyInstance(instance_, nullptr);
    }

    void renderer::init(VkSurfaceKHR surface) {
        surface_ = surface;
        devices_.physical = utils::selectPhysicalDevice(instance_, surface_);

        createLogicalDevice();

        createSwapchain();
        createRenderPass();
        createGraphicsPipeline();

        createCommandPool();
        createSyncObjects();

        std::vector<vertex> vertices = {
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
            {{0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        };

        buffer_data_info vertexBufferInfo = {};
        vertexBufferInfo.data = vertices.data();
        vertexBufferInfo.size = sizeof(vertices[0]) * vertices.size();
        vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        vertexBuffer_ = new buffer_data(devices_, vertexBufferInfo);
    }

    void renderer::draw() {
        vkWaitForFences(devices_.logical, 1, &inFlightFences_[frame_], VK_TRUE, UINT64_MAX);
        vkResetFences(devices_.logical, 1, &inFlightFences_[frame_]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(devices_.logical, swapchain_->value, UINT64_MAX, imageAvailableSemaphores_[frame_], VK_NULL_HANDLE, &imageIndex);

        record(*vertexBuffer_, imageIndex);

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores_[frame_]};
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores_[imageIndex]};

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commands_->buffers[imageIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(queues_->graphics[0], 1, &submitInfo, inFlightFences_[frame_]);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain_->value;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(queues_->present[0], &presentInfo);
        frame_ = (frame_ + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void renderer::record(const buffer_data& buffer, uint32_t index) {
        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = {
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        clearValues[1].depthStencil = {1.0f, 0};

        commands_->begin(index);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderpass_->value;
        renderPassInfo.framebuffer = renderpass_->framebuffers[index];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain_->extent;
        renderPassInfo.clearValueCount = clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commands_->buffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdSetViewport(commands_->buffers[index], 0, 1, &viewport_);
        vkCmdSetScissor(commands_->buffers[index], 0, 1, &scissor_);
        vkCmdBindPipeline(commands_->buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);

        VkBuffer vertexBuffers[] = {buffer.value};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commands_->buffers[index], 0, 1, vertexBuffers, offsets);

        vkCmdDraw(commands_->buffers[index], static_cast<uint32_t>(buffer.size), 1, 0, 0);

        vkCmdEndRenderPass(commands_->buffers[index]);
        commands_->end(index);
    }

    void renderer::createInstance() {
        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.enabledExtensionCount = static_cast<uint32_t>(info_.extensions.size());
        instanceInfo.ppEnabledExtensionNames = info_.extensions.data();

        if (validationLayersEnabled) {
            if (!utils::isValidationLayersSupported()) {
                throw std::runtime_error("Vulkan | validation layers requested, but not available!");
            }

            instanceInfo.enabledLayerCount = utils::validationLayers.size();
            instanceInfo.ppEnabledLayerNames = utils::validationLayers.data();
        }

        if (vkCreateInstance(&instanceInfo, nullptr, &instance_) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create instance!");
        }
    }

    void renderer::createLogicalDevice() {
        queue_families_support_details families = utils::queryQueueFamiliesSupport(devices_.physical, surface_);

        // prepare queue creation info for each unique family
        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        for (const auto& family : families.collect()) {
            if (!family->isReady()) {
                continue;
            }

            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = family->index;
            queueInfo.queueCount = family->count;
            queueInfo.pQueuePriorities = family->priorities.data();

            queueInfos.push_back(queueInfo);
        }

        // todo: specify device features (enable compute, tessellation, etc.)
        VkPhysicalDeviceFeatures deviceFeatures{};
        // deviceFeatures.c = VK_TRUE;

        // create logical device
        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
        deviceInfo.pQueueCreateInfos = queueInfos.data();
        deviceInfo.enabledExtensionCount = static_cast<uint32_t>(utils::deviceExtensions.size());
        deviceInfo.ppEnabledExtensionNames = utils::deviceExtensions.data();
        deviceInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(devices_.physical, &deviceInfo, nullptr, &devices_.logical) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create logical device!");
        }

        queues_ = std::make_unique<queues_data>(devices_.logical, families);
    }

    void renderer::createSwapchain() {
        swapchain_support_details details = utils::querySwapchainSupport(devices_.physical, surface_);
        if (details.formats.empty() || details.modes.empty()) {
            throw std::runtime_error("Vulkan | swapchain not supported!");
        }

        // select best options
        VkSurfaceFormatKHR surfaceFormat = utils::selectSurfaceFormat(details.formats);

        VkSwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainInfo.surface = surface_;
        swapchainInfo.minImageCount = utils::selectSurfaceImageCount(details.capabilities);
        swapchainInfo.imageFormat = surfaceFormat.format;
        swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainInfo.imageExtent = utils::selectSurfaceExtent(width_, height_, details.capabilities);
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainInfo.preTransform = details.capabilities.currentTransform;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = utils::selectSurfacePresentMode(details.modes);
        swapchainInfo.clipped = VK_TRUE;
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        swapchain_ = std::make_unique<swapchain_data>(devices_.logical, swapchainInfo);

        viewport_.x = 0.0f;
        viewport_.y = 0.0f;
        viewport_.minDepth = 0.0f;
        viewport_.maxDepth = 1.0f;
        viewport_.width = static_cast<float>(swapchain_->extent.width);
        viewport_.height = static_cast<float>(swapchain_->extent.height);

        scissor_.offset = {0, 0};
        scissor_.extent = swapchain_->extent;
    }

    void renderer::createRenderPass() {
        depthData_ = std::make_unique<image_buffer_data>(devices_, image_buffer_data_info{
                                                                       helpers::findDepthFormat(devices_.physical),
                                                                       swapchain_->extent,
                                                                       VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                                   });

        color_attachment_info color(swapchain_->format, 0);
        depth_attachment_info depth(depthData_->format, 1);

        // subpass
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color.reference;
        subpass.pDepthStencilAttachment = &depth.reference;

        // subpass dependencies (layout transitions)
        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        // create render pass
        std::array<VkAttachmentDescription, 2> attachments = {color.description, depth.description};

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &subpassDependency;

        renderpass_ = std::make_unique<renderpass_data>(*swapchain_, renderPassInfo, depthData_->view);
    }

    void renderer::createGraphicsPipeline() {
        // load shaders
        auto vertShaderCode = utils::read_file("shaders/vert.spv");
        auto fragShaderCode = utils::read_file("shaders/frag.spv");

        VkShaderModule vertShaderModule;
        VkShaderModuleCreateInfo vertShaderInfo = {};
        vertShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertShaderInfo.codeSize = vertShaderCode.size();
        vertShaderInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());

        if (vkCreateShaderModule(devices_.logical, &vertShaderInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create vertex shader module!");
        }

        VkShaderModule fragShaderModule;
        VkShaderModuleCreateInfo fragShaderInfo = {};
        fragShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragShaderInfo.codeSize = fragShaderCode.size();
        fragShaderInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());

        if (vkCreateShaderModule(devices_.logical, &fragShaderInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create fragment shader module!");
        }

        // shader stage info

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {
            vertShaderStageInfo,
            fragShaderStageInfo,
        };
        // fixed-function stages
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        // location 0: pos
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(vertex, pos);

        // location 1: color
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(vertex, color);

        // location 2: normal
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(vertex, normal);

        // location 3: uv
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(vertex, uv);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport_;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor_;

        VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
        rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerStateInfo.depthClampEnable = VK_FALSE;
        rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizerStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizerStateInfo.depthBiasEnable = VK_FALSE;
        rasterizerStateInfo.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
        multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisamplingStateInfo.sampleShadingEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
        depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateInfo.depthTestEnable = VK_TRUE;
        depthStencilStateInfo.depthWriteEnable = VK_TRUE;
        depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateInfo.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachmentStateInfo = {};
        colorBlendAttachmentStateInfo.blendEnable = VK_FALSE;
        colorBlendAttachmentStateInfo.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
        colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendingStateInfo.logicOpEnable = VK_FALSE;
        colorBlendingStateInfo.attachmentCount = 1;
        colorBlendingStateInfo.pAttachments = &colorBlendAttachmentStateInfo;

        // pipeline layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(devices_.logical, &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create pipeline layout!");
        }

        // create graphics pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizerStateInfo;
        pipelineInfo.pMultisampleState = &multisamplingStateInfo;
        pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
        pipelineInfo.pColorBlendState = &colorBlendingStateInfo;
        pipelineInfo.layout = pipelineLayout_;
        pipelineInfo.renderPass = renderpass_->value;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(devices_.logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create graphics pipeline!");
        }

        // cleanup shader modules
        vkDestroyShaderModule(devices_.logical, fragShaderModule, nullptr);
        vkDestroyShaderModule(devices_.logical, vertShaderModule, nullptr);
    }

    void renderer::createCommandPool() {
        // todo: support other family queues
        queue_families_support_details families = utils::queryQueueFamiliesSupport(devices_.physical, surface_);

        commands_ = std::make_unique<commands_pool_data>(devices_.logical, families.graphics, swapchain_->views.size());
    }

    void renderer::createSyncObjects() {
        imageAvailableSemaphores_.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores_.resize(swapchain_->images.size());
        inFlightFences_.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkCreateSemaphore(devices_.logical, &semaphoreInfo, nullptr, &imageAvailableSemaphores_[i]);
        }

        for (size_t i = 0; i < swapchain_->images.size(); i++) {
            vkCreateSemaphore(devices_.logical, &semaphoreInfo, nullptr, &renderFinishedSemaphores_[i]);
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkCreateFence(devices_.logical, &fenceInfo, nullptr, &inFlightFences_[i]);
        }
    }
}