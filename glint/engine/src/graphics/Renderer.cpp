#include <cstdint>
#include <iostream>

#include <vulkan/vulkan.hpp>

#include "glint/core/Logger.h"
#include "glint/graphics/Renderer.h"
#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/VkHelpers.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/buffer/ImageBufferData.h"
#include "glint/graphics/backend/device/QueueData.h"
#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"
#include "glint/graphics/backend/renderpass/RenderpassAttachmentInfo.h"
#include "glint/graphics/backend/renderpass/RenderpassData.h"
#include "glint/graphics/backend/swapchain/SwapchainData.h"
#include "glint/graphics/backend/swapchain/SwapchainSupportDetails.h"
#include "glint/graphics/layers/InterfaceLayer.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/graphics/models/Vertex.h"
#include "glint/scene/components/CameraComponent.h"
#include "glint/utils/FileUtils.h"
#include "glint/utils/VkUtils.h"

#ifdef NDEBUG
    #define BUILD_DEBUG 0
#else
    #define BUILD_DEBUG 1
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;
const int MAX_ENTITIES = 2000;

using namespace glint::engine::core;
using namespace glint::engine::utils;

namespace glint::engine::graphics {

    Renderer::Renderer(int width, int height, const std::vector<const char*>& extensions) : m_width(width), m_height(height) {
        VulkanVersion loaderVersion = getVulkanLoaderVersion();
        LOG_DEBUG("Vulkan loader supports version {}.{}.{}", loaderVersion.major, loaderVersion.minor, loaderVersion.patch);

        createInstance(extensions);
    }

    Renderer::~Renderer() {
        vkDeviceWaitIdle(m_devices.logical);

        for (auto& frame : m_frames)
            frame.reset();

        for (VkFramebuffer fb : renderpass->framebuffers)
            vkDestroyFramebuffer(m_devices.logical, fb, nullptr);

        vkDestroyImageView(m_devices.logical, depthBuffer->view, nullptr);
        for (VkImageView view : swapchain->views)
            vkDestroyImageView(m_devices.logical, view, nullptr);

        vkDestroySwapchainKHR(m_devices.logical, swapchain->handle, nullptr);

        vkDestroyImage(m_devices.logical, depthBuffer->image, nullptr);
        vkFreeMemory(m_devices.logical, depthBuffer->memory, nullptr);

        vkDestroyPipeline(m_devices.logical, m_pipeline, nullptr);
        vkDestroyPipelineLayout(m_devices.logical, m_pipelineLayout, nullptr);
        vkDestroyRenderPass(m_devices.logical, renderpass->handle, nullptr);

        vkDestroyDescriptorSetLayout(m_devices.logical, cameraLayout, nullptr);
        vkDestroyDescriptorSetLayout(m_devices.logical, entityLayout, nullptr);
        vkDestroyDescriptorPool(m_devices.logical, descriptorPool, nullptr);

        vkFreeCommandBuffers(m_devices.logical, commands->handle, commands->buffers.size(), commands->buffers.data());
        vkDestroyCommandPool(m_devices.logical, commands->handle, nullptr);

        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyDevice(m_devices.logical, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void Renderer::init(const VkSurfaceKHR& surface) {
        m_surface = surface;

        createDevices();

        VulkanVersion deviceVersion = getDeviceVulkanVersion(m_devices.physical);
        LOG_DEBUG("Vulkan physical device supports version {}.{}.{}", deviceVersion.major, deviceVersion.minor, deviceVersion.patch);

        createSwapchain();
        createRenderPass();

        { // todo: cleanup
            std::array<VkDescriptorPoolSize, 2> poolSizes = {};
            poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
            poolSizes[1] = {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_FRAMES_IN_FLIGHT};

            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * 2;

            if (vkCreateDescriptorPool(m_devices.logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create descriptor pool!");
            }
        }

        createCameraLayout();
        createEntityLayout();
        createGraphicsPipeline();

        createCommandPool();
        createSyncObjects();
    }

    void Renderer::begin() noexcept {
        const auto& frame = m_frames[m_frame];

        vkWaitForFences(m_devices.logical, 1, &m_frames[m_frame]->m_guard, VK_TRUE, UINT64_MAX);
        vkResetFences(m_devices.logical, 1, &m_frames[m_frame]->m_guard);

        vkAcquireNextImageKHR(m_devices.logical, swapchain->handle, UINT64_MAX, m_frames[m_frame]->m_ready, VK_NULL_HANDLE, &m_image);

        frame->begin();
    }

    void Renderer::record(float deltaTime, const CameraSnapshot& snapshot) noexcept {
        const auto& frame = m_frames[m_frame];

        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        commands->begin(m_frame);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderpass->handle;
        renderPassInfo.framebuffer = renderpass->framebuffers[m_frame];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->extent;
        renderPassInfo.clearValueCount = clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commands->buffers[m_frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdSetViewport(commands->buffers[m_frame], 0, 1, &m_viewport);
        vkCmdSetScissor(commands->buffers[m_frame], 0, 1, &m_scissor);

        FrameRenderInfo frameInfo = {commands->buffers[m_frame], m_pipeline, m_pipelineLayout, snapshot};
        frame->render(deltaTime, frameInfo);

        vkCmdEndRenderPass(commands->buffers[m_frame]);
        commands->end(m_frame);
    }

    void Renderer::end() noexcept {
        const auto& frame = m_frames[m_frame];

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        std::vector<VkSemaphore> waitSemaphores = {frame->m_ready};
        std::vector<VkSemaphore> signalSemaphores = {frame->m_done};

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commands->buffers[m_frame];
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        vkQueueSubmit(queues->graphics[0], 1, &submitInfo, frame->m_guard);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain->handle;
        presentInfo.pImageIndices = &m_image;
        vkQueuePresentKHR(queues->present[0], &presentInfo);

        frame->end();
        m_frame = (m_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::createInstance(const std::vector<const char*>& extensions) {
        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.enabledExtensionCount = extensions.size();
        instanceInfo.ppEnabledExtensionNames = extensions.data();

        if (BUILD_DEBUG) {
            // LOG_DEBUG("Vulkan validation layers requested!");

            if (!utils::isValidationLayersSupported()) {
                throw std::runtime_error("Vulkan | validation layers requested, but not available!");
            }

            instanceInfo.enabledLayerCount = utils::validationLayers.size();
            instanceInfo.ppEnabledLayerNames = utils::validationLayers.data();
        }

        if (vkCreateInstance(&instanceInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create instance!");
        }
    }

    void Renderer::createDevices() {
        m_devices.physical = utils::selectPhysicalDevice(m_instance, m_surface);

        QueueFamiliesSupportDetails families = utils::queryQueueFamiliesSupport(m_devices.physical, m_surface);

        // prepare queue creation info for each unique family
        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        for (const auto& family : families.collect()) {
            if (!family->available()) {
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

        // create logical device
        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
        deviceInfo.pQueueCreateInfos = queueInfos.data();
        deviceInfo.enabledExtensionCount = static_cast<uint32_t>(utils::deviceExtensions.size());
        deviceInfo.ppEnabledExtensionNames = utils::deviceExtensions.data();
        deviceInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(m_devices.physical, &deviceInfo, nullptr, &m_devices.logical) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create logical device!");
        }

        queues = std::make_unique<QueuesData>(m_devices.logical, families);
    }

    void Renderer::createSwapchain() {
        SwapchainSupportDetails details = utils::querySwapchainSupport(m_devices.physical, m_surface);
        if (details.formats.empty() || details.modes.empty()) {
            throw std::runtime_error("Vulkan | swapchain not supported!");
        }

        // select best options
        VkSurfaceFormatKHR surfaceFormat = utils::selectSurfaceFormat(details.formats);

        VkSwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainInfo.surface = m_surface;
        swapchainInfo.minImageCount = utils::selectSurfaceImageCount(details.capabilities);
        swapchainInfo.imageFormat = surfaceFormat.format;
        swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainInfo.imageExtent = utils::selectSurfaceExtent(m_width, m_height, details.capabilities);
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainInfo.preTransform = details.capabilities.currentTransform;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = utils::selectSurfacePresentMode(details.modes);
        swapchainInfo.clipped = VK_TRUE;
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        swapchain = std::make_unique<SwapchainData>(m_devices.logical, swapchainInfo);

        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;
        m_viewport.width = static_cast<float>(swapchain->extent.width);
        m_viewport.height = static_cast<float>(swapchain->extent.height);

        m_scissor.offset = {0, 0};
        m_scissor.extent = swapchain->extent;
    }

    void Renderer::createRenderPass() {
        ImageBufferDataInfo depthInfo = ImageBufferDataInfo{findDepthFormat(m_devices.physical), swapchain->extent};

        depthBuffer = std::make_unique<ImageBufferData>(m_devices, depthInfo);

        ColorAttachmentInfo color(swapchain->format, 0);
        DepthAttachmentInfo depth(depthBuffer->format, 1);

        // subpass
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color.m_reference;
        subpass.pDepthStencilAttachment = &depth.m_reference;

        // subpass dependencies (layout transitions)
        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        // create render pass
        std::array<VkAttachmentDescription, 2> attachments = {color.m_description, depth.m_description};

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &subpassDependency;

        renderpass = std::make_unique<RenderpassData>(*swapchain, renderPassInfo, depthBuffer->view);
    }

    void Renderer::createGraphicsPipeline() {
        // load shaders
        auto vertShaderCode = files::read("shaders/vert.spv");
        auto fragShaderCode = files::read("shaders/frag.spv");

        VkShaderModule vertShaderModule;
        VkShaderModuleCreateInfo vertShaderInfo = {};
        vertShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertShaderInfo.codeSize = vertShaderCode.size();
        vertShaderInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());

        if (vkCreateShaderModule(m_devices.logical, &vertShaderInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create vertex shader module!");
        }

        VkShaderModule fragShaderModule;
        VkShaderModuleCreateInfo fragShaderInfo = {};
        fragShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragShaderInfo.codeSize = fragShaderCode.size();
        fragShaderInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());

        if (vkCreateShaderModule(m_devices.logical, &fragShaderInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
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
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        // location 0: pos
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        // location 1: color
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // location 2: normal
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);

        // location 3: uv
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, uv);

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
        viewportState.pViewports = &m_viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &m_scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
        rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerStateInfo.depthClampEnable = VK_FALSE;
        rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerStateInfo.cullMode = VK_CULL_MODE_NONE;
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
        colorBlendAttachmentStateInfo.colorWriteMask
            = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
        colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendingStateInfo.logicOpEnable = VK_FALSE;
        colorBlendingStateInfo.attachmentCount = 1;
        colorBlendingStateInfo.pAttachments = &colorBlendAttachmentStateInfo;

        // pipeline layout
        std::vector<VkDescriptorSetLayout> layouts = {cameraLayout, entityLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
        pipelineLayoutInfo.pSetLayouts = layouts.data();

        if (vkCreatePipelineLayout(m_devices.logical, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
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
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = renderpass->handle;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_devices.logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create graphics pipeline!");
        }

        // cleanup shader modules
        vkDestroyShaderModule(m_devices.logical, fragShaderModule, nullptr);
        vkDestroyShaderModule(m_devices.logical, vertShaderModule, nullptr);
    }

    void Renderer::createCommandPool() {
        // todo: support other family queues
        QueueFamiliesSupportDetails families = utils::queryQueueFamiliesSupport(m_devices.physical, m_surface);

        commands = std::make_unique<CommandsPoolData>(m_devices.logical, families.graphics, MAX_FRAMES_IN_FLIGHT);
    }

    void Renderer::createSyncObjects() {
        m_frames.resize(MAX_FRAMES_IN_FLIGHT);

        FrameCreateInfo frameInfo = {descriptorPool, cameraLayout, entityLayout};

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            m_frames[i] = std::make_unique<FrameData>(m_devices, frameInfo);

            for (int j = 0; j < layers.size(); ++j) {
                m_frames[i]->attach(layers[j].get());
            }
        }
    }

    void Renderer::createCameraLayout() {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;

        if (vkCreateDescriptorSetLayout(m_devices.logical, &layoutInfo, nullptr, &cameraLayout) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to create camera layout!");
        }
    }

    void Renderer::createEntityLayout() {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;

        if (vkCreateDescriptorSetLayout(m_devices.logical, &layoutInfo, nullptr, &entityLayout) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to create entity layout!");
        }
    }

}
