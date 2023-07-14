#pragma once

#include <string>
#include "stb/stb_image.h"

#define IMAGE_LOAD(...) stbi_load(__VA_ARGS__)

class ResLoader {
public:
	static void flipImagesVertical(bool flip);
	static std::string loadText(const char* path);
};