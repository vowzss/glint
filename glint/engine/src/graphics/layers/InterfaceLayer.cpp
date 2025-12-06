#include <stdexcept>

#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/graphics/layers/InterfaceLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace glint::engine::graphics {

    namespace layers {
        InterfaceLayer::InterfaceLayer(const backend::DeviceContext& devices, InterfaceLayerInfo info) : device(devices.logical) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGui_ImplGlfw_InitForVulkan(info.window, true);

            VkDescriptorPoolSize poolSizes[] = {
                {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
            };

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
            poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
            poolInfo.pPoolSizes = poolSizes;

            if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create ImGui descriptor pool!");
            }

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = info.instance;
            initInfo.PhysicalDevice = devices.physical;
            initInfo.Device = device;
            initInfo.QueueFamily = info.queueFamily;
            initInfo.Queue = info.queue;
            initInfo.DescriptorPool = descriptorPool;
            initInfo.MinImageCount = 2;
            initInfo.ImageCount = info.imageCount;
            initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            initInfo.RenderPass = info.renderPass;
            ImGui_ImplVulkan_Init(&initInfo);

            /*VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = info.pool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to begin recording command buffer!");
            }

            //ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to end recording command buffer!");
            }

            VkSubmitInfo endInfo = {};
            endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            endInfo.commandBufferCount = 1;
            endInfo.pCommandBuffers = &commandBuffer;
            vkQueueSubmit(info.queue, 1, &endInfo, VK_NULL_HANDLE);

            vkDeviceWaitIdle(device);
            //ImGui_ImplVulkan_DestroyFontUploadObjects();*/
        }

        InterfaceLayer::~InterfaceLayer() {
            vkDeviceWaitIdle(device);

            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            if (descriptorPool != VK_NULL_HANDLE) {
                vkDestroyDescriptorPool(device, descriptorPool, nullptr);
            }
        }

        void InterfaceLayer::begin() {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();
            ImGui::ShowDemoWindow();
        }

        void InterfaceLayer::render(float deltaTime, const VkCommandBuffer& commands) {
            ImGui::Render();
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commands);
        }
    }
}