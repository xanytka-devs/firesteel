#pragma once

#include <string>
#include "stb/stb_image.hpp"

class ResLoader {
public:
	static void flipImagesVertical(bool flip);
	static stbi_uc* loadImage(char const* file, int* width, int* height, int* comp, int req_comp);

	static std::string loadText(const char* path);
};