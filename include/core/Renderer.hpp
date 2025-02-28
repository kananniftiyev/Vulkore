#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "vk/VulkanMain.hpp"
#include "vk/VulkanImgui.hpp"
#include "vk/VulkanCommon.hpp"
#include "vk/VulkanWindow.hpp"
#include <chrono>

#define RUN_MAIN_LOOP(window)              \
    while (!glfwWindowShouldClose(window)) \
    {                                      \
                                           \
        glfwPollEvents();                  \
    }

struct RendererContext
{
    VK::VulkanContext vulkan;
    VK::ImguiContext imgui;
    VK::Window::WindowContext window;
};

class Renderer
{
private:
    RendererContext context;

public:
    Renderer();

    /// @brief This function run before *Start*, where vulkan and window initialized. Awake func is good for configs and etc.
    void Awake();

    /// @brief Start function runs right before main loop. This function is good for object creating or memory allocations before starting drawing.
    void Start();

    /// @brief Update function is runing every frame. This is where render and logic happens.
    void Update();

    /// @brief Cleanup function for created stuff at start of program.
    void Shutdown();
};
