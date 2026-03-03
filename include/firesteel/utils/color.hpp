#ifndef FS_UTIL_COLOR_H
#define FS_UTIL_COLOR_H

#include <firesteel/common.hpp>

namespace Color {
    glm::vec3 UIntToRGB(const unsigned int& tR, const unsigned int& tG, const unsigned int& tB) {
        return glm::vec3(tR/255,tG/255,tB/255);
    }
    glm::vec3 HexToRGB(const char* tHex) {
        if(tHex[0] == '#') {
            return glm::vec3(
                std::stoul(std::string("0x") + tHex[1] + tHex[2], nullptr, 4),
                std::stoul(std::string("0x") + tHex[3] + tHex[4], nullptr, 4),
                std::stoul(std::string("0x") + tHex[5] + tHex[6], nullptr, 4)
            );
        } else {
            return glm::vec3(
                std::stoul(std::string("0x") + tHex[0] + tHex[1], nullptr, 4),
                std::stoul(std::string("0x") + tHex[2] + tHex[3], nullptr, 4),
                std::stoul(std::string("0x") + tHex[4] + tHex[5], nullptr, 4)
            );
        }
    }
    glm::vec3 CMYKToRGB(const unsigned int& tC, const unsigned int& tM, const unsigned int& tY, const unsigned int& tK) {
        return glm::vec3(
            (unsigned char)(255 * (1 - tC) * (1 - tK)),
            (unsigned char)(255 * (1 - tM) * (1 - tK)),
            (unsigned char)(255 * (1 - tY) * (1 - tK))
        );
    }
    glm::vec4 UIntToRGBA(const unsigned int tR, const unsigned int& tG, const unsigned int& tB, const unsigned int& tA) {
        return glm::vec4(tR/255,tG/255,tB/255,tA/255);
    }
    glm::vec4 HexToRGBA(const char* tHex) {
        if(tHex[0] == '#') {
            return glm::vec4(
                std::stoul(std::string("0x") + tHex[1] + tHex[2], nullptr, 4),
                std::stoul(std::string("0x") + tHex[3] + tHex[4], nullptr, 4),
                std::stoul(std::string("0x") + tHex[5] + tHex[6], nullptr, 4),
                std::stoul(std::string("0x") + tHex[7] + tHex[8], nullptr, 4)
            );
        } else {
            return glm::vec4(
                std::stoul(std::string("0x") + tHex[0] + tHex[1], nullptr, 4),
                std::stoul(std::string("0x") + tHex[2] + tHex[3], nullptr, 4),
                std::stoul(std::string("0x") + tHex[4] + tHex[5], nullptr, 4),
                std::stoul(std::string("0x") + tHex[6] + tHex[7], nullptr, 4)
            );
        }
    }
}

#endif // !FS_UTIL_COLOR_H