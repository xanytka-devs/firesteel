
#ifndef IO_OS_H
#define IO_OS_H

#include <string>

namespace XEngine {
#define XENGINE_IO
	class OS {
	public:
		static std::string open_file_dialog(std::string t_default = "", std::string t_filter = "All\0");

		enum WindowStyle {
			WS_NONE = 0x0,
			WS_NO_MAXIMIZE = 0x1,
			WS_NO_MINIMIZE = 0x2,
			WS_NO_RESIZE = 0x3
		};
		static void set_window_style(WindowStyle t_style);
	};
}

#endif // IO_OS_H