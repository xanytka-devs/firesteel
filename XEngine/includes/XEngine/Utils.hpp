#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

namespace XEngine {
	class Utils {
	public:
		static std::string read_from_file(std::string t_path);
		static bool write_file(std::string t_path, std::string t_content);
		static std::vector<std::string> split_str(std::string* t_str, char t_splitter);
	};
}

#endif // UTILS_H