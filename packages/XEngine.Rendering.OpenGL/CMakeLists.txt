cmake_minimum_required(VERSION 3.12)
project(XEngine.Rendering.OpenGL)
set(P_RENDERING_OPENGL XEngine.Rendering.OpenGL)

target_include_directories(${P_RENDERING_OPENGL} PUBLIC includes)
target_include_directories(${P_RENDERING_OPENGL} PRIVATE src)