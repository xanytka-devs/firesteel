#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

namespace XEngine {
	enum RenderMode {
		DEFAULT = 0,
		WIREFRAME
	};

	class Renderer {
	public:
		static void initialize();
		static bool initialize_libs();
		static void terminate();
		static void switch_mode(RenderMode t_mode);
		static double get_time();
		static void set_clear_color(glm::vec3 t_rgb);
		static void set_clear_color(glm::vec4 t_argb);
		static glm::vec4 get_clear_color();
	private:
		static glm::vec4 m_color;
	};
}

#endif // RENDERER_H