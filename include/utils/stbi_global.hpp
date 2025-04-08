#ifndef STBI_GLOBAL
#define STBI_GLOBAL

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

/// Loads texture from given file.
unsigned int TextureFromFile(const std::string& tPath, bool* tIsMonochromeOut = nullptr, bool tGamma = false) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(tPath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
        if (nrComponents == 1) {
            format = GL_RED;
            *tIsMonochromeOut = true;
        }
        else if (nrComponents == 3)
            tGamma ? format = GL_RGB : format = GL_SRGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else LOG_ERRR("Couldn't load texture at: \"" + tPath + "\"");
    stbi_image_free(data);
    return textureID;
}

struct TextureData {
    unsigned char* data;
    int width, height, nrComponents;
};

TextureData TextureDataFromFile(const std::string& tPath) {
    int width = 0, height = 0, nrComponents = 0;
    return TextureData{ stbi_load(tPath.c_str(), &width, &height, &nrComponents, 0), width, height, nrComponents };
}

#endif // !STBI_GLOBAL