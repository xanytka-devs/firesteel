#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

namespace firesteel {
	/// <summary>
	/// Read all text from file.
	/// </summary>
	/// <param name="t_path">Path to file.</param>
	/// <returns>All text from file.</returns>
	static std::string read_from_file(std::string t_path) {
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
		else LOG_ERRR("Couldn't load file at: '", t_path.c_str(), "'.");
		file.close();
		return output;
	}
	/// <summary>
	/// Write to file.
	/// </summary>
	/// <param name="t_path">Path to file.</param>
	/// <param name="t_content">All text.</param>
	/// <returns>Sucess of writing.</returns>
	static bool write_file(std::string t_path, std::string t_content) {
		//Create variables.
		std::ofstream out;
		bool output = true;
		//Open/create file and try to write.
		out.open(t_path);
		if(out.is_open())
			out << t_content;
		//If failed - acess denied.
		else {
			LOG_ERRR("Couldn't write in file at: '", t_path.c_str(), "'.");
			output = false;
		}
		//Close and return sucess.
		out.close();
		return output;
	}

	/// <summary>
	/// Split string by char.
	/// </summary>
	/// <param name="t_str">Base string.</param>
	/// <param name="t_splitter">Char cplitter.</param>
	/// <returns>Vector of strings.</returns>
	static std::vector<std::string> split_str(std::string* t_str, char t_splitter) {
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

	/// <summary>
	/// Check if string ends with suffix.
	/// </summary>
	/// <param name="t_str">String.</param>
	/// <param name="t_suffix">Suffix to check for.</param>
	/// <returns>Does string end with suffix.</returns>
	static bool ends_with(const char* t_str, const char* t_suffix) {
		size_t len = strlen(t_str);
		size_t suffixlen = strlen(t_suffix);
		if(suffixlen > len) return false;

		t_str += (len - suffixlen);
		return strcmp(t_str, t_suffix) == 0;
	}
}

#endif // UTILS_H