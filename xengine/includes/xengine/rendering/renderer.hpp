#ifndef RENDERER_H
#define RENDERER_H

#include <string>
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
		static void print_host_info();
		static std::string get_vendor();
		static std::string get_renderer();
		static std::string get_version();

		static void set_clear_color(float t_r, float t_g, float t_b, float t_a = 1);
		static void set_clear_color(glm::vec3 t_rgb);
		static void set_clear_color(glm::vec4 t_argb);
		static glm::vec4 get_clear_color();
	private:
		static glm::vec4 m_color;
	};
}

#endif // RENDERER_H