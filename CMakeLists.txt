cmake_minimum_required(VERSION 3.19.0)
project(Vulkore VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_DEPENDS_GRAPHVIZ "deps.dot")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/Kanan/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file" FORCE)



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



if(UNIX)
    find_package(glfw3 REQUIRED)
    find_package(Vulkan REQUIRED)
    find_package(glm REQUIRED)
endif(UNIX)

if(WIN32)
    FetchContent_Declare(
      GLM 
      GIT_REPOSITORY https://github.com/g-truc/glm.git
      GIT_TAG        master
    )

    FetchContent_Declare(
      GLFW 
      GIT_REPOSITORY https://github.com/glfw/glfw.git
      GIT_TAG        master
    )

FetchContent_MakeAvailable(GLM)
FetchContent_MakeAvailable(GLFW)

set(VULKAN_SDK "C:/VulkanSDK/1.4.304.1")

include_directories("${VULKAN_SDK}/Include")

    link_directories("${VULKAN_SDK}/Lib")
     link_directories("${VULKAN_SDK}/Lib")

    # Set Vulkan library for Windows
    set(VULKAN_LIB "${VULKAN_SDK}/Lib/vulkan-1.lib")
    include_directories("${GLFW_SOURCE_DIR}/include")

endif(WIN32)



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
    ${IMGUI_BACKENDS}  
)

include_directories(include)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends  
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
    vk-bootstrap
    VulkanMemoryAllocator
    imgui
)

if(WIN32)
    target_link_libraries(Vulkore PRIVATE ${VULKAN_LIB})
endif(WIN32)


if(UNIX)
 target_link_libraries(Vulkore PRIVATE     Vulkan::Vulkan
)
endif(UNIX)