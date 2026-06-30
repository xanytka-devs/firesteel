#ifndef STBI_GLOBAL_H
#define STBI_GLOBAL_H

#include <string>

/// Loads texture from given file.
unsigned int TextureFromFile(const std::string& tPath, bool* tIsMonochromeOut = nullptr, bool tGamma = false);

struct TextureData {
    unsigned char* data;
    int width, height, nrComponents;
};

TextureData TextureDataFromFile(const std::string& tPath);

extern "C" {
    typedef unsigned char stbi_uc;
    typedef unsigned short stbi_us;
    typedef void stbi_write_func(void* context, void* data, int size);

    void stbi_image_free(void* retval_from_stbi_load);
    stbi_uc* stbi_load_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp, int req_comp);
    stbi_us* stbi_load_16_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
    int stbi_is_16_bit_from_memory(stbi_uc const* buffer, int len);
    int stbi_info_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp);
    int stbi_write_png_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int stride_bytes);
    int stbi_write_bmp_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data);
    int stbi_write_jpg_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int quality);
    stbi_uc* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp);
}

#endif // !STBI_GLOBAL_H