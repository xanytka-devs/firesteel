#include <algorithm>
#include <cmath>
#include <glad/glad.h>

#include "XEngine/Rendering/Texture.hpp"

namespace XEngine::Rendering {

    GLenum TextureFormat_to_Format(TextureFormat format) {
        switch (format) {
            case XEngine::Rendering::Red8:
                return GL_R8;
            case XEngine::Rendering::RGB8:
                return GL_RGB8;
            case XEngine::Rendering::RGBA8:
                return GL_RGBA8;
            case XEngine::Rendering::SRGB8:
                return GL_SRGB8;
            case XEngine::Rendering::SRGBA8:
                return GL_SRGB8_ALPHA8;
            case XEngine::Rendering::BGR:
                return GL_BGR;
            case XEngine::Rendering::BGRA:
                return GL_BGRA;
            case XEngine::Rendering::Red16:
                return GL_R16;
            case XEngine::Rendering::RGB16:
                return GL_RGB16;
            case XEngine::Rendering::RGBA16:
                return GL_RGBA16;
            case XEngine::Rendering::RGB:
                return GL_RGB;
            case XEngine::Rendering::RGBA:
                return GL_RGBA;
            case XEngine::Rendering::SRGB:
                return GL_SRGB;
            case XEngine::Rendering::SRGBA:
                return GL_SRGB_ALPHA;
            default:
                return GL_RGB;
        }
    }

    Texture2D::Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height,
        const TextureFormat internalFormat, const TextureFormat baseFormat)
        : width(width), height(height) {
        glCreateTextures(GL_TEXTURE_2D, 1, &curID);
        const GLsizei mip_levels = static_cast<GLsizei>(std::log2(std::max(width, height))) + 1;
        glTextureStorage2D(curID, mip_levels, TextureFormat_to_Format(internalFormat), width, height);
        glTextureSubImage2D(curID, 0, 0, 0, width, height, TextureFormat_to_Format(baseFormat), GL_UNSIGNED_BYTE, data);
        glTextureParameteri(curID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(curID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(curID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(curID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateTextureMipmap(curID);
    }
    Texture2D::~Texture2D() {
        glDeleteTextures(1, &curID);
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept {
        glDeleteTextures(1, &curID);
        curID = texture.curID;
        width = texture.width;
        height = texture.height;
        texture.curID = 0;
        return *this;
    }
    Texture2D::Texture2D(Texture2D&& texture) noexcept {
        curID = texture.curID;
        width = texture.width;
        height = texture.height;
        texture.curID = 0;
    }

    void Texture2D::bind(const unsigned int unit) const {
        glBindTextureUnit(unit, curID);
    }
}