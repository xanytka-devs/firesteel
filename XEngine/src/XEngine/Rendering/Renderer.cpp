#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "XEngine/Rendering/Renderer.hpp"
#include "XEngine/Log.hpp"

namespace XEngine {

	glm::vec4 Renderer::m_color(0.15f, 0.15f, 0.15f, 1);

	void Renderer::initialize() {
		glfwInit();
		//Set version to 3.3.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//Set profile.
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	}

	bool Renderer::initialize_libs() {
		//Load GLAD.
		return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void Renderer::terminate() {
		glfwTerminate();
	}

	void Renderer::switch_mode(RenderMode t_mode) {
		switch (t_mode) {
		case DEFAULT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
	}

	double Renderer::get_time() {
		return glfwGetTime();
	}

	void Renderer::set_clear_color(float t_r, float t_g, float t_b, float t_a) {
		m_color = glm::vec4(t_r, t_g, t_b, t_a);
	}
	void Renderer::set_clear_color(glm::vec3 t_rgb) {
		m_color = glm::vec4(t_rgb, 1);
	}
	void Renderer::set_clear_color(glm::vec4 t_argb) {
		m_color = t_argb;
	}
	glm::vec4 Renderer::get_clear_color() {
		return m_color;
	}

}