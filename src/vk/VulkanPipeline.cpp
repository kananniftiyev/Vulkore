#include "vk/VulkanPipeline.hpp"

VkPipeline VK::createPipeline(VkDevice &device, VkPipelineLayout &layout, VkRenderPass &renderpass, VulkanPipeline &pipe)
{
    VkPipelineViewportStateCreateInfo vp{};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.viewportCount = 1;
    vp.pViewports = &pipe.viewport;
    vp.scissorCount = 1;
    vp.pScissors = &pipe.rect;

    VkPipelineColorBlendStateCreateInfo color_blend{};
    color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend.logicOpEnable = VK_FALSE;
    color_blend.logicOp = VK_LOGIC_OP_COPY;
    color_blend.attachmentCount = 1;
    color_blend.pAttachments = &pipe.color_blend;

    VkGraphicsPipelineCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.stageCount = pipe.stages.size();
    info.pStages = pipe.stages.data();
    info.pVertexInputState = &pipe.input_state;
    info.pInputAssemblyState = &pipe.assembly_state;
    info.pViewportState = &vp;
    info.pInputAssemblyState = &pipe.assembly_state;
    info.pRasterizationState = &pipe.rasterization_state;
    info.pMultisampleState = &pipe.multisample;
    info.pDepthStencilState = &pipe.depth;
    info.pColorBlendState = &color_blend;
    info.basePipelineHandle = VK_NULL_HANDLE;
    info.subpass = 0;

    VkPipeline new_pipeline;

    auto res = vkCreateGraphicsPipelines(device, nullptr, 1, &info, nullptr, &new_pipeline);

    if (res != VK_SUCCESS)
    {
        std::cerr << "Could not create pipeline";
        return VK_NULL_HANDLE;
    }

    return new_pipeline;
}