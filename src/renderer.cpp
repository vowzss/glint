#include "glint/renderer.h"

#include <cstdint>

#include <GLFW/glfw3.h>
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
#include "vulkan/vulkan_core.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

using namespace glint;

renderer::renderer(const resolution_info& res, const renderer_info& info)
    : res_(res), info_(info) {}

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

void renderer::init(GLFWwindow* window) {
    createInstance();
    createSurface(window);

    devices_.physical = utils::selectPhysicalDevice(instance_, surface_);
    createLogicalDevice();

    createSwapchain();
    createRenderPass();
    createGraphicsPipeline();

    createCommandPool();
    createSyncObjects();

    std::vector<vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    };

    buffer_data_info vertexBufferInfo{
        .data = vertices.data(),
        .size = sizeof(vertices[0]) * vertices.size(),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

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

    VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &commands_->buffers[imageIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };

    vkQueueSubmit(queues_->graphics[0], 1, &submitInfo, inFlightFences_[frame_]);

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = &swapchain_->value,
        .pImageIndices = &imageIndex,
    };

    vkQueuePresentKHR(queues_->present[0], &presentInfo);
    frame_ = (frame_ + 1) % MAX_FRAMES_IN_FLIGHT;
}

void renderer::record(const buffer_data& buffer, uint32_t index) {
    std::array<VkClearValue, 2> clearValues;
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    commands_->begin(index);

    VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = renderpass_->value,
        .framebuffer = renderpass_->framebuffers[index],
        .renderArea{{0, 0}, swapchain_->extent},
        .clearValueCount = clearValues.size(),
        .pClearValues = clearValues.data(),
    };

    vkCmdBeginRenderPass(commands_->buffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(commands_->buffers[index], 0, 1, &viewport);
    vkCmdSetScissor(commands_->buffers[index], 0, 1, &scissor);
    vkCmdBindPipeline(commands_->buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);

    VkBuffer vertexBuffers[] = {buffer.value};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commands_->buffers[index], 0, 1, vertexBuffers, offsets);

    vkCmdDraw(commands_->buffers[index], static_cast<uint32_t>(buffer.size), 1, 0, 0);

    vkCmdEndRenderPass(commands_->buffers[index]);
    commands_->end(index);
}

void renderer::createInstance() {
    VkInstanceCreateInfo instanceInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .enabledExtensionCount = static_cast<uint32_t>(info_.extensions.size()),
        .ppEnabledExtensionNames = info_.extensions.data(),
    };

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

void renderer::createSurface(GLFWwindow* window) {
    if (glfwCreateWindowSurface(instance_, window, nullptr, &surface_) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create surface!");
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

        VkDeviceQueueCreateInfo queueInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family->index,
            .queueCount = family->count,
            .pQueuePriorities = family->priorities.data(),
        };

        queueInfos.push_back(queueInfo);
    }

    // todo: specify device features (enable compute, tessellation, etc.)
    VkPhysicalDeviceFeatures deviceFeatures{};
    // deviceFeatures.c = VK_TRUE;

    // create logical device
    VkDeviceCreateInfo deviceInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size()),
        .pQueueCreateInfos = queueInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(utils::deviceExtensions.size()),
        .ppEnabledExtensionNames = utils::deviceExtensions.data(),
        .pEnabledFeatures = &deviceFeatures,
    };

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

    VkSwapchainCreateInfoKHR swapchainInfo{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface_,
        .minImageCount = utils::selectSurfaceImageCount(details.capabilities),
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = utils::selectSurfaceExtent(res_, details.capabilities),
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = details.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = utils::selectSurfacePresentMode(details.modes),
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE,
    };

    swapchain_ = std::make_unique<swapchain_data>(devices_.logical, swapchainInfo);

    viewport.width = static_cast<float>(swapchain_->extent.width);
    viewport.height = static_cast<float>(swapchain_->extent.height);

    scissor.extent = swapchain_->extent;
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
    VkSubpassDescription subpass{
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color.reference,
        .pDepthStencilAttachment = &depth.reference,
    };

    // subpass dependencies (layout transitions)
    VkSubpassDependency subpassDependency{
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };

    // create render pass
    std::array<VkAttachmentDescription, 2> attachments = {color.description, depth.description};

    VkRenderPassCreateInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = attachments.size(),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };
    renderpass_ = std::make_unique<renderpass_data>(*swapchain_, renderPassInfo, depthData_->view);
}

void renderer::createGraphicsPipeline() {
    // load shaders
    auto vertShaderCode = utils::read_file("shaders/vert.spv");
    auto fragShaderCode = utils::read_file("shaders/frag.spv");

    VkShaderModule vertShaderModule;
    VkShaderModuleCreateInfo vertShaderInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = vertShaderCode.size(),
        .pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data()),
    };

    if (vkCreateShaderModule(devices_.logical, &vertShaderInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create vertex shader module!");
    }

    VkShaderModule fragShaderModule;
    VkShaderModuleCreateInfo fragShaderInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = fragShaderCode.size(),
        .pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data()),
    };

    if (vkCreateShaderModule(devices_.logical, &fragShaderInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create fragment shader module!");
    }

    // shader stage info
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName = "main",
    };

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName = "main",
    };

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo,
        fragShaderStageInfo,
    };

    // fixed-function stages
    VkVertexInputBindingDescription bindingDescription{
        .binding = 0,
        .stride = sizeof(vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

    // location 0: pos
    attributeDescriptions[0] = {
        .location = 0,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(vertex, pos),
    };

    // location 1: color
    attributeDescriptions[1] = {
        .location = 1,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(vertex, color),
    };

    // location 2: normal
    attributeDescriptions[2] = {
        .location = 2,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(vertex, normal),
    };

    // location 3: uv
    attributeDescriptions[3] = {
        .location = 3,
        .binding = 0,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = offsetof(vertex, uv),
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &bindingDescription,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions = attributeDescriptions.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineViewportStateCreateInfo viewportState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor,
    };

    VkPipelineRasterizationStateCreateInfo rasterizerStateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisamplingStateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
    };

    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachmentStateInfo{
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachmentStateInfo,
    };

    // pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0,
    };

    if (vkCreatePipelineLayout(devices_.logical, &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create pipeline layout!");
    }

    // create graphics pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shaderStages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizerStateInfo,
        .pMultisampleState = &multisamplingStateInfo,
        .pDepthStencilState = &depthStencilStateInfo,
        .pColorBlendState = &colorBlendingStateInfo,
        .layout = pipelineLayout_,
        .renderPass = renderpass_->value,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
    };

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

    VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    VkFenceCreateInfo fenceInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

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