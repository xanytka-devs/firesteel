cmake_minimum_required(VERSION 3.12)
project(XEngine.Rendering.Vulkan)
set(P_RENDERING_VULKAN XEngine.Rendering.Vulkan)
#TODO: Create package.

target_include_directories(${P_RENDERING_VULKAN} PUBLIC includes)
target_include_directories(${P_RENDERING_VULKAN} PRIVATE src)