#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "xengine/common.hpp"
#include "xengine/math.hpp"

struct GLFWwindow;

namespace XEngine {
	enum RenderMode {
		DEFAULT = 0x0,
		WIREFRAME = 0x1
	};

	class Renderer {
	public:
		/// <summary>
		/// Get renderer's availability.
		/// </summary>
		/// <returns>Availability.</returns>
		static bool is_available();
		/// <summary>
		/// Initialize renderer.
		/// </summary>
		static void initialize();
		/// <summary>
		/// Initialize renderer's libs.
		/// </summary>
		/// <param name="t_window">Window.</param>
		/// <returns>Sucess of initialization.</returns>
		static bool initialize_libs(GLFWwindow* t_window);
		/// <summary>
		/// Shutdown renderer.
		/// </summary>
		static void terminate();

		/// <summary>
		/// Switch rendering mode.
		/// </summary>
		/// <param name="t_mode">Rendering mode.</param>
		static void switch_mode(RenderMode t_mode);
		/// <summary>
		/// Get current time.
		/// </summary>
		/// <returns>Current time.</returns>
		static double get_time();
		/// <summary>
		/// Print info about host.
		/// </summary>
		static void print_host_info();
		/// <summary>
		/// Get rederer's vendor.
		/// </summary>
		/// <returns>Rederer's vendor.</returns>
		static const char* get_vendor();
		/// <summary>
		/// Get rederer's name.
		/// </summary>
		/// <returns>Rederer's name.</returns>
		static const char* get_renderer();
		/// <summary>
		/// Get rederer's version.
		/// </summary>
		/// <returns>Rederer's version.</returns>
		static const char* get_version();

		/// <summary>
		/// Set renderer's clear color.
		/// </summary>
		/// <param name="t_r">Red.</param>
		/// <param name="t_g">Green.</param>
		/// <param name="t_b">Blue.</param>
		/// <param name="t_a">Alpha (Opacity).</param>
		static void set_clear_color(float t_r, float t_g, float t_b, float t_a = 1)
			{ m_color = glm::vec4(t_r, t_g, t_b, t_a); }
		/// <summary>
		/// Set renderer's clear color.
		/// </summary>
		/// <param name="t_rgb">New clear color.</param>
		static void set_clear_color(glm::vec3 t_rgb)
			{ m_color = glm::vec4(t_rgb, 1); }
		/// <summary>
		/// Set renderer's clear color.
		/// </summary>
		/// <param name="t_argb">New clear color.</param>
		static void set_clear_color(glm::vec4 t_argb)
			{ m_color = t_argb; }
		/// <summary>
		/// Get renderer's clear color.
		/// </summary>
		/// <returns>Clear color.</returns>
		static glm::vec4 get_clear_color()
			{ return m_color; }
	private:
		static glm::vec4 m_color;
	};
}

#endif // RENDERER_H