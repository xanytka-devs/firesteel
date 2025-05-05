#ifndef FS_COMMON_H
#define FS_COMMON_H
#include <../external/glad/include/glad/gl.h>
#include <../external/glfw/include/GLFW/glfw3.h>
#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif // !WIN32
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#endif // !__linux__
#include "../external/glfw/include/GLFW/glfw3native.h"
#include <../external/glm/glm/glm.hpp>

#include "utils/log.hpp"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#endif // !FS_COMMON_H