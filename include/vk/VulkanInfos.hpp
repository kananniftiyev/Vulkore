#pragma once

#include <vulkan/vulkan.h>

namespace VK::Info
{
    VkCommandPoolCreateInfo createCommandPoolInfo(uint32_t queue_family, VkCommandPoolCreateFlags flag);
    VkCommandBufferAllocateInfo createCommandBufferInfo(VkCommandPool command_pool);
    VkSubmitInfo createSubmitInfo(VkSemaphore *wait_semaphore, VkPipelineStageFlags &wait_stage_mask, VkCommandBuffer &cmd, VkSemaphore *signal_semaphore);
    VkImageCreateInfo createImageInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo createImageViewInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
    VkRenderPassBeginInfo createRenderPassInfo(VkRenderPass &renderpass, VkFramebuffer &framebuffer, VkClearValue clear_values[2], VkExtent2D &swapchain_extend);
    VkPresentInfoKHR createPresentInfo(VkSwapchainKHR &swapchain, uint32_t &swapchainIndex, VkSemaphore &waitSemaphore);

    // Pipeline
    VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderModule &module, VkShaderStageFlagBits &stage);
    // VkPipelineVertexInputStateCreateInfo createVertexInputStage();
    VkPipelineInputAssemblyStateCreateInfo createInputAssemblyInfo();
    VkPipelineTessellationStateCreateInfo createTesselationInfo(uint32_t patch_control_points);
    VkPipelineRasterizationStateCreateInfo createRasterizationInfo(VkCullModeFlags cull_mode);
    VkPipelineMultisampleStateCreateInfo createMultisampleInfo(VkBool32 sample_shading, VkSampleCountFlagBits sample_count);
    VkPipelineDepthStencilStateCreateInfo createDepthInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);

    VkRenderingAttachmentInfo createAttachmentInfo(VkImageView view, VkClearValue *clear, VkImageLayout layout /*= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL*/);
} // namespace VK::Info
