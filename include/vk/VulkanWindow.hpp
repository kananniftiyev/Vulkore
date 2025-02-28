#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

namespace VK::Window
{
    struct WindowContext
    {
        uint32_t width = 800;
        uint32_t height = 600;
        bool is_fullscren = false;
        bool hdr = false;
        GLFWwindow *window;
    };

    WindowContext initWindow(uint32_t width, uint32_t height);
    bool setFullscreen();
    void shutdown(WindowContext &context);

} // namespace VK::Window
