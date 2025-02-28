#define VMA_IMPLEMENTATION
#include "vk/VulkanMain.hpp"

VK::VulkanContext VK::initVulkan(GLFWwindow *&window)
{
    VulkanContext context{};
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("Vulkan")
                        .enable_validation_layers(true)
                        .require_api_version(1, 3, 0)
                        .use_default_debug_messenger()
                        .build();

    auto vkb_inst = inst_ret.value();

    context.instance = vkb_inst.instance;
    context.debug_messenger = vkb_inst.debug_messenger;

    if (window == nullptr)
    {
        throw std::runtime_error("Window handle is null");
    }

    if (context.instance == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Vulkan instance is null");
    }

    context.surface = VK_NULL_HANDLE;

    VkResult result = glfwCreateWindowSurface(context.instance, window, nullptr, &context.surface);
    if (result != VK_SUCCESS)
    {
        switch (result)
        {
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            throw std::runtime_error("Required surface extension not present");
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            throw std::runtime_error("Window surface is already in use");
        default:
            throw std::runtime_error("Failed to create window surface: " + std::to_string(result));
        }
    }
    vkb::PhysicalDeviceSelector selector{vkb_inst};
    auto physical_device = selector.set_minimum_version(1, 3)
                               .set_surface(context.surface)
                               .select()
                               .value();

    vkb::DeviceBuilder device_builder{physical_device};

    auto vkb_device = device_builder.build().value();

    context.device = vkb_device.device;
    context.physical_device = vkb_device.physical_device;

    context.graphics_queue = vkb_device.get_queue(vkb::QueueType::graphics).value();
    context.graphics_queue_intex = vkb_device.get_queue_index(vkb::QueueType::graphics).value();

    // VMA allocator
    VmaAllocatorCreateInfo allocator_info = {};
    allocator_info.physicalDevice = context.physical_device;
    allocator_info.device = context.device;
    allocator_info.instance = context.instance;
    VK_CHECK(vmaCreateAllocator(&allocator_info, &context.allocator));

    // Swapchain

    vkb::SwapchainBuilder swapchain_builder{context.physical_device, context.device, context.surface};

    vkb::Swapchain vkb_swapchain = swapchain_builder
                                       .use_default_format_selection()
                                       // use vsync present mode
                                       .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                                       .set_desired_extent(context.swapchain_extend.width, context.swapchain_extend.height)
                                       .build()
                                       .value();

    // store swapchain and its related images
    context.swapchain = vkb_swapchain.swapchain;
    context.swapchain_images = vkb_swapchain.get_images().value();
    context.swapchain_image_view = vkb_swapchain.get_image_views().value();

    context.swapchain_format = vkb_swapchain.image_format;

    // Depth
    context.depth_extend = {
        context.swapchain_extend.width,
        context.swapchain_extend.height,
        1};

    context.depth_format = VK_FORMAT_D32_SFLOAT;

    auto dimg_info = VK::Info::createImageInfo(context.depth_format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, context.depth_extend);

    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK(vmaCreateImage(context.allocator, &dimg_info, &dimg_allocinfo, &context.depth_image.image, &context.depth_image.allocation, nullptr));

    auto dview_info = VK::Info::createImageViewInfo(context.depth_format, context.depth_image.image, VK_IMAGE_ASPECT_DEPTH_BIT);

    VK_CHECK(vkCreateImageView(context.device, &dview_info, nullptr, &context.depth_image_view)); // error

    // Commands
    auto command_pool_info = VK::Info::createCommandPoolInfo(context.graphics_queue_intex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.pNext = nullptr;

    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;

    for (int i = 0; i < FRAME_OVERLAP; i++)
    {
        VK_CHECK(vkCreateCommandPool(context.device, &command_pool_info, nullptr, &context.frames[i].command_pool));

        auto comman_buffer_info = VK::Info::createCommandBufferInfo(context.frames[i].command_pool);

        VK_CHECK(vkAllocateCommandBuffers(context.device, &comman_buffer_info, &context.frames[i].render_command));
        VK_CHECK(vkAllocateCommandBuffers(context.device, &comman_buffer_info, &context.frames[i].ui_command));

        VK_CHECK(vkCreateFence(context.device, &fence_info, nullptr, &context.frames[i].render_fence));

        VK_CHECK(vkCreateSemaphore(context.device, &info, nullptr, &context.frames[i].swapchain_semaphore));
        VK_CHECK(vkCreateSemaphore(context.device, &info, nullptr, &context.frames[i].render_semaphore));
    }

    // Render pass
    VkAttachmentDescription color_attachment{};
    color_attachment.format = context.swapchain_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Depth
    VkAttachmentDescription depth_attachment{};
    depth_attachment.flags = 0;
    depth_attachment.format = context.depth_format;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // we are going to create 1 subpass, which is the minimum you can do
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkAttachmentDescription attachments[2] = {color_attachment, depth_attachment};

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency depth_dependency = {};
    depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    depth_dependency.dstSubpass = 0;
    depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.srcAccessMask = 0;
    depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency dependencies[2] = {dependency, depth_dependency};

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    render_pass_info.attachmentCount = 2;
    render_pass_info.pAttachments = &attachments[0];
    // connect the subpass to the info
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 2;
    render_pass_info.pDependencies = &dependencies[0];

    VK_CHECK(vkCreateRenderPass(context.device, &render_pass_info, nullptr, &context.renderpass));

    // Frame Buffers
    VkFramebufferCreateInfo fb_info{};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = nullptr;

    fb_info.renderPass = context.renderpass;
    fb_info.attachmentCount = 1;
    fb_info.width = context.swapchain_extend.width;
    fb_info.height = context.swapchain_extend.height;
    fb_info.layers = 1;

    const uint32_t swapchain_image_count = context.swapchain_images.size();
    context.framebuffers = std::vector<VkFramebuffer>(swapchain_image_count);

    for (int i = 0; i < swapchain_image_count; i++)
    {
        VkImageView attachments[2];
        attachments[0] = context.swapchain_image_view[i];
        attachments[1] = context.depth_image_view;
        fb_info.pAttachments = attachments;
        fb_info.attachmentCount = 2;

        VK_CHECK(vkCreateFramebuffer(context.device, &fb_info, nullptr, &context.framebuffers[i]));
    }

    return context;
}

void VK::recordCommand(VkCommandBuffer cmd, std::function<void()> body)
{
    VkCommandBufferBeginInfo cmd_info{};
    cmd_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_info.pNext = nullptr;
    cmd_info.pInheritanceInfo = nullptr;
    cmd_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmd, &cmd_info);
    body();
    vkEndCommandBuffer(cmd);
}

void VK::createPipeline()
{
}

static uint32_t swapchainIndex;
static VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

// TODO: Abstract away this
// TODO: Code to pause draw comand
void VK::draw(VulkanContext &context, ImguiContext &imgui)
{
    vkWaitForFences(context.device, 1, &context.getCurrFrame().render_fence, true, 1000000000);
    vkResetFences(context.device, 1, &context.getCurrFrame().render_fence);

    vkAcquireNextImageKHR(context.device, context.swapchain, 1000000000, context.getCurrFrame().swapchain_semaphore, nullptr, &swapchainIndex);

    auto cmd = context.getCurrFrame().render_command;
    auto ui_cmd = context.getCurrFrame().ui_command;

    recordCommand(cmd, [&]
                  {
                      VkClearValue clearValue;
                      clearValue.color = {{0.0f, 0.0f, 0.0f, 0.0f}};

                      VkClearValue depthClear;
                      depthClear.depthStencil.depth = 1.f;
                      VkClearValue clearValues[] = {clearValue, depthClear};
                      auto renderpass_info = VK::Info::createRenderPassInfo(context.renderpass, context.framebuffers[swapchainIndex], clearValues, context.swapchain_extend);

                      vkCmdBeginRenderPass(cmd, &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);
                        // Objects here
                        
                      vkCmdEndRenderPass(cmd); });

    recordCommand(ui_cmd, [&]
                  {
                            VkClearValue clearValue;
                            clearValue.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
      
                            VkClearValue depthClear;
                            depthClear.depthStencil.depth = 1.f;
                            VkClearValue clearValues[] = {clearValue, depthClear};
                            auto renderpass_info = VK::Info::createRenderPassInfo(context.renderpass, context.framebuffers[swapchainIndex], clearValues, context.swapchain_extend);
      
                            vkCmdBeginRenderPass(ui_cmd, &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);
                              
                           VK::UI::drawImgui(imgui, ui_cmd); 
                              
                            vkCmdEndRenderPass(ui_cmd); });

    static VkCommandBuffer cmds[2] = {cmd, ui_cmd};

    VkSubmitInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &context.getCurrFrame().swapchain_semaphore;
    info.pWaitDstStageMask = &waitStage;
    info.commandBufferCount = 2;
    info.pCommandBuffers = cmds;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &context.getCurrFrame().render_semaphore;

    VK_CHECK(vkQueueSubmit(context.graphics_queue, 1, &info, context.getCurrFrame().render_fence));

    auto present_info = VK::Info::createPresentInfo(context.swapchain, swapchainIndex, context.getCurrFrame().render_semaphore);

    VK_CHECK(vkQueuePresentKHR(context.graphics_queue, &present_info));
}

void VK::shutdown(VulkanContext &context)
{
    vkDeviceWaitIdle(context.device);

    for (auto &frame : context.frames)
    {
        vkDestroyFence(context.device, frame.render_fence, nullptr);
        vkDestroySemaphore(context.device, frame.swapchain_semaphore, nullptr);
        vkDestroySemaphore(context.device, frame.render_semaphore, nullptr);
        vkDestroyCommandPool(context.device, frame.command_pool, nullptr);
    }

    for (auto &framebuffer : context.framebuffers)
    {
        vkDestroyFramebuffer(context.device, framebuffer, nullptr);
    }

    vkDestroyRenderPass(context.device, context.renderpass, nullptr);

    for (auto &image_view : context.swapchain_image_view)
    {
        vkDestroyImageView(context.device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(context.device, context.swapchain, nullptr);

    vkDestroyImageView(context.device, context.depth_image_view, nullptr);
    vkDestroyImage(context.device, context.depth_image.image, nullptr);
    vmaFreeMemory(context.allocator, context.depth_image.allocation);

    vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
    vmaDestroyAllocator(context.allocator);

    vkDestroyDevice(context.device, nullptr);

    vkb::destroy_debug_utils_messenger(context.instance, context.debug_messenger, nullptr);

    vkDestroyInstance(context.instance, nullptr);
}