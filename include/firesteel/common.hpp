#ifndef FS_COMMON_H
#define FS_COMMON_H
/*
* Generic internal includes
*/
// GENERAL //
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include "utils/log.hpp"
// GLM //
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
// RENDERING //
#ifdef FS_RENDERER_OGL
#include <glad/gl.h>
#endif // FS_RENDERER_OGL
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
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif // !FS_HEADLESS
// ADDITIONAL //
#ifdef FS_INCLUDE_NVTX
#include <nvtx3/nvtx3.hpp>
#endif // FS_INCLUDE_NVTX
// DEFENITIONS //
#define CASTF(...) static_cast<float>(__VA_ARGS__)
#define IFEX if constexpr
#define SAMETYPE(type,base) std::is_same<type,base>::value
#define BASEDONTYPE(type,base) std::is_base_of<base,type>::value
typedef unsigned int uint;

#endif // !FS_COMMON_H