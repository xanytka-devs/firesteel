#include <fstream>
#include <sstream>
#include <streambuf>

#include "XEngine/Log.hpp"
#include "XEngine/Utils.hpp"

namespace XEngine {

	std::string Utils::read_from_file(std::string t_path) {
		//Create variables.
		std::ifstream file;
		std::stringstream buffer;
		std::string output = "";
		//Try to read file.
		file.open(t_path);
		if (file.is_open()) {
			buffer << file.rdbuf();
			output = buffer.str();
		}
		else LOG_ERRR(("Couldn't load file at: '" + t_path + "'.").c_str());
		file.close();
		return output;

	}

}