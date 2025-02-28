#pragma once

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imconfig.h>
#include <imgui_internal.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include "vk/VulkanInfos.hpp"
#include "vk/VulkanCommon.hpp"

namespace VK::UI
{

    // DATAS
    struct DiagnosticData
    {
        int fps;
        float awake_ms;
        float start_ms;
        float update_ms;
    };

    VK::ImguiContext initImgui(GLFWwindow *window, VulkanContext &vulkan);
    void drawImgui(ImguiContext &context, VkCommandBuffer cmd);
    void shutdownImgui(ImguiContext &context, VulkanContext &vulkan);

    // windows
    void diagnosticWindow(DiagnosticData &data);
} // namespace Vulkan
