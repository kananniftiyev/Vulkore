#include "vk/VulkanInfos.hpp"

[[nodiscard]]
VkSubmitInfo VK::Info::createSubmitInfo(VkSemaphore *wait_semaphore, VkPipelineStageFlags &wait_stage_mask, VkCommandBuffer &cmd, VkSemaphore *signal_semaphore)
{
    VkSubmitInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = wait_semaphore;
    info.pWaitDstStageMask = &wait_stage_mask;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &cmd;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = signal_semaphore;

    return info;
}

[[nodiscard]]
VkCommandPoolCreateInfo VK::Info::createCommandPoolInfo(uint32_t queue_family, VkCommandPoolCreateFlags flag)
{
    VkCommandPoolCreateInfo command_pool_info{};
    command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_info.pNext = nullptr;
    command_pool_info.queueFamilyIndex = queue_family;
    command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    return command_pool_info;
}

[[nodiscard]]
VkCommandBufferAllocateInfo VK::Info::createCommandBufferInfo(VkCommandPool command_pool)
{
    VkCommandBufferAllocateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_info.pNext = nullptr;
    buffer_info.commandPool = command_pool;
    buffer_info.commandBufferCount = 2;
    buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    return buffer_info;
}

[[nodiscard]]
VkImageCreateInfo VK::Info::createImageInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent)
{
    VkImageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;

    info.imageType = VK_IMAGE_TYPE_2D;

    info.format = format;
    info.extent = extent;

    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = usageFlags;

    return info;
}

[[nodiscard]]
VkImageViewCreateInfo VK::Info::createImageViewInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext = nullptr;

    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.image = image;
    info.format = format;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    info.subresourceRange.aspectMask = aspectFlags;

    return info;
}

[[nodiscard]]
VkRenderPassBeginInfo VK::Info::createRenderPassInfo(VkRenderPass &renderpass, VkFramebuffer &framebuffer, VkClearValue clear_values[2], VkExtent2D &swapchain_extend)
{
    VkRenderPassBeginInfo info{};

    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderpass;
    info.framebuffer = framebuffer;
    info.clearValueCount = 2;
    info.pClearValues = &clear_values[0];
    info.renderArea.offset.x = 0;
    info.renderArea.offset.y = 0;
    info.renderArea.extent = swapchain_extend;

    return info;
}

[[nodiscard]]
VkPresentInfoKHR VK::Info::createPresentInfo(VkSwapchainKHR &swapchain, uint32_t &swapchainIndex, VkSemaphore &waitSemaphore)
{
    VkPresentInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.swapchainCount = 1;
    info.pSwapchains = &swapchain;
    info.pImageIndices = &swapchainIndex;
    info.pWaitSemaphores = &waitSemaphore;

    return info;
}

[[nodiscard]]
VkPipelineShaderStageCreateInfo VK::Info::createShaderStageInfo(VkShaderModule &module, VkShaderStageFlagBits &stage)
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.stage = stage;
    info.module = module;
    info.pName = "main";

    return info;
}

[[nodiscard]]
VkPipelineInputAssemblyStateCreateInfo VK::Info::createInputAssemblyInfo()
{
    VkPipelineInputAssemblyStateCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.primitiveRestartEnable = VK_FALSE;

    return info;
}

[[nodiscard]]
VkPipelineTessellationStateCreateInfo VK::Info::createTesselationInfo(uint32_t patch_control_points)
{
    VkPipelineTessellationStateCreateInfo info{};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    info.patchControlPoints = patch_control_points;

    return info;
}

[[nodiscard]]
VkPipelineRasterizationStateCreateInfo VK::Info::createRasterizationInfo(VkCullModeFlags cull_mode)
{
    VkPipelineRasterizationStateCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.depthClampEnable = VK_FALSE;
    info.rasterizerDiscardEnable = VK_FALSE;
    info.cullMode = cull_mode;
    info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    info.depthBiasEnable = VK_FALSE;
    info.depthBiasConstantFactor = 0.0f;
    info.depthBiasClamp = 0.0f;
    info.depthBiasSlopeFactor = 0.0f;

    return info;
}

[[nodiscard]]
VkPipelineMultisampleStateCreateInfo VK::Info::createMultisampleInfo(VkBool32 sample_shading, VkSampleCountFlagBits sample_count)
{
    VkPipelineMultisampleStateCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.rasterizationSamples = sample_count;
    info.sampleShadingEnable = sample_shading;
    info.minSampleShading = 1.0f;
    info.alphaToCoverageEnable = VK_FALSE;
    info.alphaToOneEnable = VK_FALSE;

    return info;
}

[[nodiscard]]
VkPipelineDepthStencilStateCreateInfo VK::Info::createDepthInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp)
{
    VkPipelineDepthStencilStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info.pNext = nullptr;

    info.depthTestEnable = bDepthTest ? VK_TRUE : VK_FALSE;
    info.depthWriteEnable = bDepthWrite ? VK_TRUE : VK_FALSE;
    info.depthCompareOp = bDepthTest ? compareOp : VK_COMPARE_OP_ALWAYS;
    info.depthBoundsTestEnable = VK_FALSE;
    info.minDepthBounds = 0.0f; // Optional
    info.maxDepthBounds = 1.0f; // Optional
    info.stencilTestEnable = VK_FALSE;

    return info;
}

VkRenderingAttachmentInfo VK::Info::createAttachmentInfo(VkImageView view, VkClearValue *clear, VkImageLayout layout)
{
    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.pNext = nullptr;

    colorAttachment.imageView = view;
    colorAttachment.imageLayout = layout;
    colorAttachment.loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    if (clear)
    {
        colorAttachment.clearValue = *clear;
    }

    return colorAttachment;
}
