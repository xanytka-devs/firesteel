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

}