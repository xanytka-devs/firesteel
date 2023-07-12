#include <algorithm>
#include <cmath>
#include <glad/glad.h>

#include "Texture2D.hpp"

namespace XEngine::OpenGL {
    Texture2D::Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height)
        : width(width)
        , height(height) {
        glCreateTextures(GL_TEXTURE_2D, 1, &curID);
        const GLsizei mip_levels = static_cast<GLsizei>(std::log2(std::max(width, height))) + 1;
        glTextureStorage2D(curID, mip_levels, GL_RGB8, width, height);
        glTextureSubImage2D(curID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
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