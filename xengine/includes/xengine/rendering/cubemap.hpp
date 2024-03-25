#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

#include "xengine/rendering/shader.hpp"
#include "xengine/enviroment.hpp"

namespace XEngine {
	class Cubemap {
	public:
		Cubemap();

		void load(const char* dir,
			const char* right = "right.png",
			const char* left = "left.png",
			const char* top = "top.png",
			const char* bottom = "bottom.png",
			const char* front = "front.png",
			const char* back = "back.png");

		void initialize();

		void render(Shader shader);

		void remove();

	private:
		unsigned int m_id;
		std::string m_dir;
		std::vector<const char*> m_faces;
		bool m_has_textures;
	};
}

#endif