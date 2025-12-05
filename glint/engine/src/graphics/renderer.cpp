#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "glint/graphics/Renderer.h"
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
#include "glint/scene/components/Camera.h"
#include "glint/utils/FileUtils.h"
#include "glint/utils/VkUtils.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace glint::engine {
    using namespace utils;
    using namespace scene;

    namespace graphics {
        using namespace backend;

        Renderer::Renderer(int width_, int height_, const std::vector<const char*>& extensions_)
            : width(width_), height(height_), context(extensions_) {
            createInstance();
        }

        Renderer::~Renderer() {
            vkDeviceWaitIdle(devices.logical);

            for (auto& frame : frames)
                frame.reset();

            for (VkFramebuffer fb : renderpass->framebuffers)
                vkDestroyFramebuffer(devices.logical, fb, nullptr);

            vkDestroyImageView(devices.logical, depthData->view, nullptr);
            for (VkImageView view : swapchain->views)
                vkDestroyImageView(devices.logical, view, nullptr);

            vkDestroySwapchainKHR(devices.logical, swapchain->value, nullptr);

            vkDestroyImage(devices.logical, depthData->image, nullptr);
            vkFreeMemory(devices.logical, depthData->memory, nullptr);

            vkDestroyPipeline(devices.logical, pipeline, nullptr);
            vkDestroyPipelineLayout(devices.logical, pipelineLayout, nullptr);
            vkDestroyRenderPass(devices.logical, renderpass->value, nullptr);

            vkDestroyDescriptorSetLayout(devices.logical, cameraDescriptorLayout, nullptr);
            vkDestroyDescriptorPool(devices.logical, descriptorPool, nullptr);

            vkFreeCommandBuffers(devices.logical, commands->value, commands->buffers.size(), commands->buffers.data());
            vkDestroyCommandPool(devices.logical, commands->value, nullptr);

            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(devices.logical, nullptr);
            vkDestroyInstance(instance, nullptr);
        }

        void Renderer::init(const VkSurfaceKHR& surface_) {
            surface = surface_;
            devices.physical = utils::selectPhysicalDevice(instance, surface);

            createLogicalDevice();

            createSwapchain();
            createRenderPass();

            { // todo: cleanup
                VkDescriptorPoolSize poolSize{};
                poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

                VkDescriptorPoolCreateInfo poolInfo{};
                poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                poolInfo.poolSizeCount = 1;
                poolInfo.pPoolSizes = &poolSize;
                poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

                if (vkCreateDescriptorPool(devices.logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
                    throw std::runtime_error("Vulkan | failed to create frame descriptor pool!");
                }
            }

            createCamera();
            createGraphicsPipeline();

            createCommandPool();
            createSyncObjects();

            layers::SceneLayerCreateInfo sceneLayerInfo{pipeline, pipelineLayout, getCamera()};
            renderLayers.emplace_back(new layers::SceneLayer(devices, sceneLayerInfo));
        }

        void Renderer::beginFrame() {
            vkWaitForFences(devices.logical, 1, &frames[frameIndex]->inFlight, VK_TRUE, UINT64_MAX);
            vkResetFences(devices.logical, 1, &frames[frameIndex]->inFlight);

            vkAcquireNextImageKHR(devices.logical, swapchain->value, UINT64_MAX, frames[frameIndex]->imageAvailable, VK_NULL_HANDLE, &imageIndex);

            const auto& frame = frames[frameIndex];
            frame->begin();
        }

        void Renderer::recordFrame(float deltaTime) {
            const auto& frame = frames[frameIndex];

            std::array<VkClearValue, 2> clearValues;
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            commands->begin(frameIndex);

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderpass->value;
            renderPassInfo.framebuffer = renderpass->framebuffers[frameIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapchain->extent;
            renderPassInfo.clearValueCount = clearValues.size();
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commands->buffers[frameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdSetViewport(commands->buffers[frameIndex], 0, 1, &viewport);
            vkCmdSetScissor(commands->buffers[frameIndex], 0, 1, &scissor);

            frame->render(deltaTime, *camera.get(), commands->buffers[frameIndex]);

            vkCmdEndRenderPass(commands->buffers[frameIndex]);
            commands->end(frameIndex);
        }

        void Renderer::endFrame() {
            const auto& frame = frames[frameIndex];

            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            VkSemaphore waitSemaphores[] = {frame->imageAvailable};
            VkSemaphore signalSemaphores[] = {frame->renderFinished};

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commands->buffers[frameIndex];
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;
            vkQueueSubmit(queues->graphics[0], 1, &submitInfo, frame->inFlight);

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain->value;
            presentInfo.pImageIndices = &imageIndex;
            vkQueuePresentKHR(queues->present[0], &presentInfo);

            frame->end();
            frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
        }

        void Renderer::createInstance() {
            VkInstanceCreateInfo instanceInfo = {};
            instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceInfo.enabledExtensionCount = context.getExtensionCount();
            instanceInfo.ppEnabledExtensionNames = context.getExtensionValues();

            if (BUILD_DEBUG) {
                // LOG_DEBUG("Vulkan validation layers requested!");

                if (!utils::isValidationLayersSupported()) {
                    throw std::runtime_error("Vulkan | validation layers requested, but not available!");
                }

                instanceInfo.enabledLayerCount = utils::validationLayers.size();
                instanceInfo.ppEnabledLayerNames = utils::validationLayers.data();
            }

            if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create instance!");
            }
        }

        void Renderer::createLogicalDevice() {
            QueueFamiliesSupportDetails families = utils::queryQueueFamiliesSupport(devices.physical, surface);

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

            // create logical device
            VkDeviceCreateInfo deviceInfo = {};
            deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
            deviceInfo.pQueueCreateInfos = queueInfos.data();
            deviceInfo.enabledExtensionCount = static_cast<uint32_t>(utils::deviceExtensions.size());
            deviceInfo.ppEnabledExtensionNames = utils::deviceExtensions.data();
            deviceInfo.pEnabledFeatures = &deviceFeatures;

            if (vkCreateDevice(devices.physical, &deviceInfo, nullptr, &devices.logical) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create logical device!");
            }

            queues = std::make_unique<QueuesData>(devices.logical, families);
        }

        void Renderer::createSwapchain() {
            SwapchainSupportDetails details = utils::querySwapchainSupport(devices.physical, surface);
            if (details.formats.empty() || details.modes.empty()) {
                throw std::runtime_error("Vulkan | swapchain not supported!");
            }

            // select best options
            VkSurfaceFormatKHR surfaceFormat = utils::selectSurfaceFormat(details.formats);

            VkSwapchainCreateInfoKHR swapchainInfo = {};
            swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainInfo.surface = surface;
            swapchainInfo.minImageCount = utils::selectSurfaceImageCount(details.capabilities);
            swapchainInfo.imageFormat = surfaceFormat.format;
            swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
            swapchainInfo.imageExtent = utils::selectSurfaceExtent(width, height, details.capabilities);
            swapchainInfo.imageArrayLayers = 1;
            swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            swapchainInfo.preTransform = details.capabilities.currentTransform;
            swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainInfo.presentMode = utils::selectSurfacePresentMode(details.modes);
            swapchainInfo.clipped = VK_TRUE;
            swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

            swapchain = std::make_unique<SwapchainData>(devices.logical, swapchainInfo);

            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            viewport.width = static_cast<float>(swapchain->extent.width);
            viewport.height = static_cast<float>(swapchain->extent.height);

            scissor.offset = {0, 0};
            scissor.extent = swapchain->extent;
        }

        void Renderer::createRenderPass() {
            ImageBufferDataInfo depthInfo = ImageBufferDataInfo{findDepthFormat(devices.physical), swapchain->extent};

            depthData = std::make_unique<ImageBufferData>(devices, depthInfo);

            ColorAttachmentInfo color(swapchain->format, 0);
            DepthAttachmentInfo depth(depthData->format, 1);

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

            renderpass = std::make_unique<RenderpassData>(*swapchain, renderPassInfo, depthData->view);
        }

        void Renderer::createGraphicsPipeline() {
            using namespace models;

            // load shaders
            auto vertShaderCode = files::read("shaders/vert.spv");
            auto fragShaderCode = files::read("shaders/frag.spv");

            VkShaderModule vertShaderModule;
            VkShaderModuleCreateInfo vertShaderInfo = {};
            vertShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            vertShaderInfo.codeSize = vertShaderCode.size();
            vertShaderInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());

            if (vkCreateShaderModule(devices.logical, &vertShaderInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create vertex shader module!");
            }

            VkShaderModule fragShaderModule;
            VkShaderModuleCreateInfo fragShaderInfo = {};
            fragShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            fragShaderInfo.codeSize = fragShaderCode.size();
            fragShaderInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());

            if (vkCreateShaderModule(devices.logical, &fragShaderInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
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
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

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
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;

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
            colorBlendAttachmentStateInfo.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

            VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
            colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlendingStateInfo.logicOpEnable = VK_FALSE;
            colorBlendingStateInfo.attachmentCount = 1;
            colorBlendingStateInfo.pAttachments = &colorBlendAttachmentStateInfo;

            VkPushConstantRange pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(JPH::Mat44);

            // pipeline layout
            VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &cameraDescriptorLayout;
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

            if (vkCreatePipelineLayout(devices.logical, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
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
            pipelineInfo.layout = pipelineLayout;
            pipelineInfo.renderPass = renderpass->value;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

            if (vkCreateGraphicsPipelines(devices.logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create graphics pipeline!");
            }

            // cleanup shader modules
            vkDestroyShaderModule(devices.logical, fragShaderModule, nullptr);
            vkDestroyShaderModule(devices.logical, vertShaderModule, nullptr);
        }

        void Renderer::createCommandPool() {
            // todo: support other family queues
            QueueFamiliesSupportDetails families = utils::queryQueueFamiliesSupport(devices.physical, surface);

            commands = std::make_unique<CommandsPoolData>(devices.logical, families.graphics, MAX_FRAMES_IN_FLIGHT);
        }

        void Renderer::createSyncObjects() {
            frames.resize(MAX_FRAMES_IN_FLIGHT);

            FrameDataCreateInfo frameInfo = {descriptorPool, cameraDescriptorLayout, entityDescriptorLayout, getCamera()};
            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                frames[i] = std::make_unique<FrameData>(devices, frameInfo);
            }
        }

        void Renderer::createCamera() {
            VkDescriptorSetLayoutBinding layoutBinding{};
            layoutBinding.binding = 0;
            layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            layoutBinding.descriptorCount = 1;
            layoutBinding.stageFlags = VK_SHADER_STAGE_ALL;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 1;
            layoutInfo.pBindings = &layoutBinding;

            if (vkCreateDescriptorSetLayout(devices.logical, &layoutInfo, nullptr, &cameraDescriptorLayout) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create camera layout!");
            }

            camera = std::make_unique<scene::components::Camera>();
        }
    }
}
