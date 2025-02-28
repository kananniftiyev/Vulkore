#include <iostream>
#include <GLFW/glfw3.h>

#include "core/Renderer.hpp"

int main()
{
    Renderer renderer{};
    renderer.Awake();
    renderer.Start();
    renderer.Update();
    renderer.Shutdown();
}
