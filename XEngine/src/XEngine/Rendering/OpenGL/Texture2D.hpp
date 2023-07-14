#pragma once

namespace XEngine::OpenGL {

    enum TextureFormat {
        Red8,
        RGB8,
        RGBA8,
        SRGB8,
        SRGBA8,
        BGR,
        BGRA,
        Red16,
        RGB16,
        RGBA16,
        RGB,
        RGBA,
        SRGB,
        SRGBA,
    };

    class Texture2D {
    public:
        Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height,
            const TextureFormat internalFormat = TextureFormat::RGB8, const TextureFormat baseFormat = TextureFormat::RGB);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture) noexcept;
        Texture2D(Texture2D&& texture) noexcept;

        void bind(const unsigned int unit) const;

    private:
        unsigned int curID = 0;
        unsigned int width = 0;
        unsigned int height = 0;
    };

}