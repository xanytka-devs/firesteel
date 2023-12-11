#ifndef RES_MANAGER_H
#define RES_MANAGER_H

#include <string>

namespace XEngine {

	class ResManager {
	public:
		static std::string read_from_file(const char* t_path);
		static unsigned char* load_image(const char* t_path, const int* t_width,
			const int* t_height, const int* t_channels, const bool t_flip_y = false);
		static void free_image(unsigned char* t_data);
	};

}

#endif // RES_MANAGER_H