#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>

#include "XEngine/Log.hpp"
#include "XEngine/ResManager.hpp"

namespace XEngine {

	std::string ResManager::read_from_file(const char* t_path) {
		//Create variables.
		std::ifstream file;
		std::stringstream buffer;
		std::string output = "";
		//Try to read file.
		file.open(t_path);
		if (file.is_open()) {
			buffer << file.rdbuf();
			output = buffer.str();
		} else {
			std::stringstream msg;
			msg << "Couldn't open file '" << t_path << "'.";
			LOG_ERRR(msg.str().c_str());
		}
		file.close();
		return output;

	}

	unsigned char* ResManager::load_image(const char* t_path, const int* t_width,
		const int* t_height, const int* t_channels, const bool t_flip_y) {
		stbi_set_flip_vertically_on_load(t_flip_y);
		unsigned char* output = NULL;
		output = stbi_load(t_path, const_cast<int*>(t_width), const_cast<int*>(t_height), const_cast<int*>(t_channels), 3);
		if(!output) {
			std::stringstream msg;
			msg << "Texture '" << t_path << "' not loaded.";
			LOG_ERRR(msg.str().c_str());
		}
		return output;
	}

	void ResManager::free_image(unsigned char* t_data) {
		stbi_image_free(t_data);
	}

}