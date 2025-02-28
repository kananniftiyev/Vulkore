#include "core/Renderer.hpp"

void Renderer::initWindow(RendererContext &context, uint32_t width, uint32_t height)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    context.window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);

    if (context.window == nullptr)
    {
        std::cerr << "Could not init window \n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(context.window);

    context.vulkan = VK::initVulkan(context.window);
    VK::UI::initImgui(context.window, context.imgui, context.vulkan);
}

void Renderer::mainloop(RendererContext &context)
{

    while (!glfwWindowShouldClose(context.window))
    {
        auto before = std::chrono::high_resolution_clock::now();

        VK::draw(context.vulkan, context.imgui);

        auto after = std::chrono::high_resolution_clock::now();

        // Convert to milliseconds
        auto frameTimeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count();
        float frameTimeMs = frameTimeNs / 1'000'000.0f; // Convert nanoseconds to milliseconds
        float fps = 1000.0f / frameTimeMs;              // FPS = 1000 / frame time (in ms)

        glfwPollEvents();
    }
}

void Renderer::shutdown(RendererContext &context)
{
    VK::UI::shutdownImgui(context.imgui, context.vulkan);
    VK::shutdown(context.vulkan);
    glfwDestroyWindow(context.window);
    glfwTerminate();
}
