#include "vk/VulkanImgui.hpp"

VK::ImguiContext VK::UI::initImgui(GLFWwindow *window, VulkanContext &vulkan)
{
    VK::ImguiContext context{};

    // Descriptor
    VkDescriptorPoolSize pool_sizes[] =
        {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    VkDescriptorPoolCreateInfo descriptor_pool_info{};
    descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptor_pool_info.maxSets = 1000;
    descriptor_pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
    descriptor_pool_info.pPoolSizes = pool_sizes;

    VK_CHECK(vkCreateDescriptorPool(vulkan.device, &descriptor_pool_info, nullptr, &context.descriptor_pool));

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info{};
    init_info.Instance = vulkan.instance;
    init_info.PhysicalDevice = vulkan.physical_device;
    init_info.Device = vulkan.device;
    init_info.QueueFamily = vulkan.graphics_queue_intex;
    init_info.Queue = vulkan.graphics_queue;
    init_info.DescriptorPool = context.descriptor_pool;
    init_info.MinImageCount = vulkan.swapchain_images.size();
    init_info.ImageCount = vulkan.swapchain_images.size();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.PipelineRenderingCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
    init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &vulkan.swapchain_format;
    init_info.RenderPass = vulkan.ui_renderpass;

    ImGui_ImplVulkan_Init(&init_info);

    ImGui_ImplVulkan_CreateFontsTexture();

    return context;
}

// TODO: Diagnotstic window
// TODO: Top bar
void VK::UI::drawImgui(ImguiContext &context, VkCommandBuffer cmd)
{

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame(); // Assuming you're using GLFW
    ImGui::NewFrame();         // This sets up the internal state for ImGui

    // windows here:
    DiagnosticData data{
        .fps = 60,
        .awake_ms = 5.0f,
        .start_ms = 5.0f,
        .update_ms = 5.0f,
    };
    diagnosticWindow(data);

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

void VK::UI::shutdownImgui(ImguiContext &context, VulkanContext &vulkan)
{
    vkDeviceWaitIdle(vulkan.device);

    // Cleanup ImGui
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(vulkan.device, context.descriptor_pool, nullptr);
}

void VK::UI::diagnosticWindow(DiagnosticData &data)
{
    if (ImGui::Begin("Profiler"))
    {

        if (ImGui::CollapsingHeader("Diagnostics", ImGuiTreeNodeFlags_AllowItemOverlap))
        {
            ImGui::Text("FPS: %d", data.fps);
            ImGui::Text("Awake: %f", data.awake_ms);
            ImGui::Text("Start: %f", data.start_ms);
            ImGui::Text("Update: %f", data.update_ms);
        }

        ImGui::End();
    }
}
