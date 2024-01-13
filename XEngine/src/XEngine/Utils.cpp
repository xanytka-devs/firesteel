#include <fstream>
#include <sstream>
#include <streambuf>

#include "xengine/log.hpp"
#include "xengine/utils.hpp"

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
		else LOG_ERRR("Couldn't load file at: '" + t_path + "'.");
		file.close();
		return output;
	}

	bool Utils::write_file(std::string t_path, std::string t_content) {
		//Create variables.
		std::ofstream out;
		bool output = true;
		//Open/create file and try to write.
		out.open(t_path);
		if(out.is_open())
			out << t_content;
		//If failed - acess denied.
		else {
			LOG_ERRR("Couldn't write in file at: '" + t_path + "'.");
			output = false;
		}
		//Close and return sucess.
		out.close();
		return output;
	}

	std::vector<std::string> Utils::split_str(std::string* t_str, char t_splitter) {
		//Create variables.
		std::stringstream test(*t_str);
		std::string segment;
		std::vector<std::string> seglist;
		//Get line and segment.
		while(std::getline(test, segment, t_splitter))
			seglist.push_back(segment);
		//Return vector.
		return seglist;
	}

}