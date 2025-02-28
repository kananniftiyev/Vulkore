cmake_minimum_required(VERSION 3.5.0)
project(Vulkore VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_DEPENDS_GRAPHVIZ "deps.dot")


include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        docking
)

FetchContent_Declare(
    vk-bootstrap
  GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
  GIT_TAG        main
)

FetchContent_Declare(
  VulkanMemoryAllocator 
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(vk-bootstrap)
FetchContent_MakeAvailable(VulkanMemoryAllocator)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)


file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(Vulkore ${SRC_FILES})

# Add ImGui backends
set(IMGUI_BACKENDS
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

# Create ImGui library
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKENDS}  # Add backend sources
)

include_directories(include)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends  # Important: Backends need this!
)

target_include_directories(Vulkore PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${VulkanMemoryAllocator}
    ${glm_INCLUDE_DIRS}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(Vulkore PRIVATE
    glfw
    Vulkan::Vulkan
    vk-bootstrap
    VulkanMemoryAllocator
    imgui
)