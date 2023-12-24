#ifndef RES_MANAGER_H
#define RES_MANAGER_H

namespace XEngine {

	class ResManager {
	public:
		static std::string read_from_file(std::string t_path);
		static unsigned char* load_image(std::string t_path, const int* t_width,
			const int* t_height, const int* t_channels, const bool t_flip_y = false);
		static void free_image(unsigned char* t_data);
	};

}

#endif // RES_MANAGER_H