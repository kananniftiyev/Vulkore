#include <iostream>
#include <GLFW/glfw3.h>

#include "core/Renderer.hpp"

int main()
{
    Renderer::RendererContext renderer{};

    Renderer::initWindow(renderer, 800, 600);

    if (renderer.window == nullptr)
    {
        std::cout << "/* message */" << std::endl;
        return -1;
    }

    Renderer::mainloop(renderer);
    Renderer::shutdown(renderer);
}
