#ifndef RENDERER_H
#define RENDERER_H

//Select current rendering engines.
#define OPENGL_RENDERING // DONE
#define VULKAN_RENDERING // TODO

#include <string>
#include "xengine/math.hpp"

namespace XEngine {
	enum RenderMode {
		DEFAULT = 0x0,
		WIREFRAME = 0x1
	};

	class Renderer {
	public:
		static bool is_available();
		static void initialize();
		static bool initialize_libs();
		static void terminate();

		static void switch_mode(RenderMode t_mode);
		static double get_time();
		static void print_host_info();
		static std::string get_vendor();
		static std::string get_renderer();
		static std::string get_version();

		static void set_clear_color(float t_r, float t_g, float t_b, float t_a = 1)
			{ m_color = glm::vec4(t_r, t_g, t_b, t_a); }
		static void set_clear_color(glm::vec3 t_rgb)
			{ m_color = glm::vec4(t_rgb, 1); }
		static void set_clear_color(glm::vec4 t_argb)
			{ m_color = t_argb; }
		static glm::vec4 get_clear_color()
			{ return m_color; }
	private:
		static glm::vec4 m_color;
	};
}

#endif // RENDERER_H