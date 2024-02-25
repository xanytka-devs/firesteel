
#ifndef IO_OS_H
#define IO_OS_H

#include <string>

namespace XEngine {
#define XENGINE_IO
	class OS {
	public:
		static std::string open_file_dialog(std::string t_default = "", std::string t_filter = "All\0");
	};
}

#endif // IO_OS_H