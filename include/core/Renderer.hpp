#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "vk/VulkanMain.hpp"
#include "vk/VulkanImgui.hpp"
#include "vk/VulkanCommon.hpp"
#include <chrono>

struct VulkanContext;

namespace Renderer
{
    struct RendererContext
    {
        bool HDR = false;
        bool fullscreen = false;
        GLFWwindow *window;

        VK::VulkanContext vulkan;
        VK::ImguiContext imgui;
    };

    // TODO: Fullscreen
    void initWindow(RendererContext &context, uint32_t width, uint32_t height);
    void mainloop(RendererContext &context);
    void shutdown(RendererContext &context);

} // namespace Renderer
