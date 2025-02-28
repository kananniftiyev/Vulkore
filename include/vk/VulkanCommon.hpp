#pragma once

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <vector>
#include <iostream>
#include <array>

#define VK_CHECK(x)                                                     \
    do                                                                  \
    {                                                                   \
        VkResult err = x;                                               \
        if (err)                                                        \
        {                                                               \
            std::cerr << "Detected Vulkan error: " << err << std::endl; \
            abort();                                                    \
        }                                                               \
    } while (0)

namespace VK
{

    constexpr int FRAME_OVERLAP = 2;

    struct FrameData
    {
        VkCommandPool command_pool;
        VkCommandBuffer render_command, ui_command;

        VkSemaphore swapchain_semaphore, render_semaphore;
        VkFence render_fence;
    };

    struct AllocatedImage
    {
        VkImage image;
        VmaAllocation allocation;
    };

    struct ImguiContext
    {
        VkDescriptorPool descriptor_pool;
    };

    struct VulkanContext
    {
        VkInstance instance;
        VkDebugUtilsMessengerEXT debug_messenger;
        VkPhysicalDevice physical_device;
        VkDevice device;
        VkQueue graphics_queue;
        uint32_t graphics_queue_intex;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;
        std::vector<VkImage> swapchain_images;
        std::vector<VkImageView> swapchain_image_view;
        VkFormat swapchain_format;
        VkExtent2D swapchain_extend = {.width = 800, .height = 600};
        VkFormat depth_format;
        VkExtent3D depth_extend;
        VkImageView depth_image_view;
        AllocatedImage depth_image;

        // Memory management
        VmaAllocator allocator;

        VkRenderPass renderpass;
        VkRenderPass ui_renderpass;
        std::vector<VkFramebuffer> framebuffers;
        std::vector<VkFramebuffer> ui_framebuffers;

        int framecount = 0;
        std::array<FrameData, FRAME_OVERLAP> frames;
        FrameData &getCurrFrame() { return frames[framecount % FRAME_OVERLAP]; }
    };
} // namespace VK
