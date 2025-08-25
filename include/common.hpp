#ifndef FS_COMMON_H
#define FS_COMMON_H
/*
* Generic internal includes
*/
#ifdef FS_RENDERER_OGL
#include <../external/glad/include/glad/gl.h>
#endif // FS_HEADLESS
#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif // WIN32
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#endif // __linux__
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif // __APPLE__
#ifndef FS_HEADLESS
#include <../external/glfw/include/GLFW/glfw3.h>
#include "../external/glfw/include/GLFW/glfw3native.h"
#endif // !FS_HEADLESS

#include <../external/glm/glm/glm.hpp>

#include "utils/log.hpp"

#ifdef FS_INCLUDE_NVTX
#include <nvtx3/nvtx3.hpp>
#endif // FS_INCLUDE_NVTX

#endif // !FS_COMMON_H