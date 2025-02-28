#include "vk/VulkanWindow.hpp"

namespace VK::Window
{
    WindowContext initWindow(uint32_t width, uint32_t height)
    {
        WindowContext ctx{};
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        ctx.window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
        ctx.width = width;
        ctx.height = height;

        if (ctx.window == nullptr)
        {
            std::cerr << "Could not init window \n";
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(ctx.window);

        return ctx;
    }
}

bool VK::Window::setFullscreen()
{
    return false;
}

void VK::Window::shutdown(WindowContext &context)
{
    glfwDestroyWindow(context.window);
    glfwTerminate();
}
