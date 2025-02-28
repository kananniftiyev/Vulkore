#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"
#include <vector>
#include "VkBootstrap.h"
#include <iostream>
#include "vk/VulkanInfos.hpp"
#include <functional>
#include "vk/VulkanCommon.hpp"
#include "vk/VulkanImgui.hpp"

namespace VK
{

    VulkanContext initVulkan(GLFWwindow *&window);
    void draw(VulkanContext &context, ImguiContext &imgui);
    void recordCommand(VkCommandBuffer cmd, std::function<void()> body);
    void createPipeline();
    // TODO: Fullscreen
    // void recreateSwapchain();
    void shutdown(VulkanContext &context);

} // namespace VK
