#pragma once

#include <string>

namespace XEngine {

	class ResManager {
	public:
		static std::string read_from_file(const char* t_path);
	};

}