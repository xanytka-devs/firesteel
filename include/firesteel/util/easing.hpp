#ifndef FS_UTIL_EASING_H
#define FS_UTIL_EASING_H

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace Easing {
    float linear(float v) { return v; }
    float quadIn(float v) { return v * v; }
    float quadOut(float v) { return -v * (v - 2); }
    float quadInOut(float v) { return v < 0.5 ? 2 * v * v : -2 * v * v + 4 * v - 1; }
    float cubicIn(float v) { return v * v * v; }
    float cubicOut(float v) { double t = v - 1; return t * t * t + 1; }
    float cubicInOut(float v) { return v < 0.5 ? 2 * v * v : -2 * v * v + 4 * v - 1; }
    float quartIn(float v) { return v * v * v * v; }
    float quartOut(float v) { return 1 - glm::pow(1 - v, 4); }
    float quartInOut(float v) { return v < 0.5 ? 8 * v * v * v * v : 1 - glm::pow(-2 * v + 2, 4) / 2; }
    float sineIn(float v) { return 1 - glm::cos((v * glm::pi<float>()) / 2); }
    float sineOut(float v) { return glm::sin((v * glm::pi<float>()) / 2); }
    float sineInOut(float v) { return -0.5 * (glm::cos(glm::pi<float>() * v) - 1); }
    float expoIn(float v) { return v == 0 ? 0 : glm::pow(2, 10 * (v - 1)); }
    float expoOut(float v) { return v == 1 ? 1 : 1 - glm::pow(2, -10 * v); }
    float expoInOut(float v) { return v == 0 ? 0.0 : v == 1 ? 1.0 : v < 0.5 ? 0.5 * glm::pow(2, 20 * v - 10) : -0.5 * glm::pow(2, -20 * v + 10) + 1; }
    float circIn(float v) { return 1 - glm::sqrt(1 - glm::pow(v, 2)); }
    float circOut(float v) { return glm::sqrt(1 - glm::pow(v - 1, 2)); }
    float circInOut(float v) { return v < 0.5 ? (1 - glm::sqrt(1 - glm::pow(2 * v, 2))) / 2 : (glm::sqrt(1 - glm::pow(-2 * v + 2, 2)) + 1) / 2; }
}

#endif // !FS_UTIL_EASING_H