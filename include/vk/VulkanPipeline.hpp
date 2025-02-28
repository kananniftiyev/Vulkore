#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace VK
{
    struct VulkanPipeline
    {
        VkViewport viewport;
        VkRect2D rect;
        std::vector<VkPipelineShaderStageCreateInfo> stages;
        VkPipelineVertexInputStateCreateInfo input_state;
        VkPipelineInputAssemblyStateCreateInfo assembly_state;
        VkPipelineTessellationStateCreateInfo tesselation_state;
        VkPipelineRasterizationStateCreateInfo rasterization_state;
        VkPipelineMultisampleStateCreateInfo multisample;
        VkPipelineDepthStencilStateCreateInfo depth;
        VkPipelineColorBlendAttachmentState color_blend;
    };

    VkPipeline createPipeline(VkDevice &device, VkPipelineLayout &layout, VkRenderPass &renderpass, VulkanPipeline &pipe);

} // namespace VK
