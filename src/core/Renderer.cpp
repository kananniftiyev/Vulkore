#include "core/Renderer.hpp"

Renderer::Renderer()
{
    context.window = VK::Window::initWindow(800, 600);
    context.vulkan = VK::initVulkan(context.window.window);
    context.imgui = VK::UI::initImgui(context.window.window, context.vulkan);
}

void Renderer::Awake()
{
}

void Renderer::Start()
{
}

void Renderer::Update()
{
    while (!glfwWindowShouldClose(context.window.window))
    {
        VK::draw(context.vulkan, context.imgui);
        glfwPollEvents();
    }
}

void Renderer::Shutdown()
{
    VK::UI::shutdownImgui(context.imgui, context.vulkan);
    VK::shutdown(context.vulkan);
    VK::Window::shutdown(context.window);
}