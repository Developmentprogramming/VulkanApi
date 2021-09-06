cmake_minimum_required(VERSION 3.19)
project(VulkanApi)

set(CMAKE_CXX_STANDARD 17)
set(LINK_ARGS "-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi")

if (VULKAN_API_DEBUG)
    add_definitions(-DVULKAN_API_DEBUG=1)
endif(VULKAN_API_DEBUG)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${LINK_ARGS})
add_executable(VulkanApi src/main.cpp src/Core/Instance.cpp src/Core/Instance.h src/Core/Window.cpp src/Core/Window.h src/Core/Surface.cpp src/Core/Surface.h src/Core/Device.cpp src/Core/Device.h src/Core/SwapChain.cpp src/Core/SwapChain.h src/Core/Queue.cpp src/Core/Queue.h)