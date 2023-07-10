#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "XEngine/Log.hpp"

namespace XEngine::OpenGL {

	/// <summary>
	/// Initializes OpenGLs rendering context.
	/// </summary>
	/// <param name="pWindow">Window reference.</param>
	/// <returns>True if initialization is successful.</returns>
	bool Renderer::init(GLFWwindow* pWindow) {
		//Make the window's context current.
		glfwMakeContextCurrent(pWindow);
		//Check if GLAD is loaded.
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			LOG_CRIT("Failed to initialize GLAD (OpenGL).");
			return false;
		}
		//Log info.
		LOG_INFO("OpenGL context created.");
		LOG_INFO("	Vendor: {0}", getVendorStr());
		LOG_INFO("	Renderer: {0}", getRendererStr());
		LOG_INFO("	Version: {0}", getVendorStr());
		return true;
	}

	void Renderer::draw(const VertexArray& vertexArray) {
		vertexArray.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer::setClearColorRGB(const float r, const float g, const float b) {
		glClearColor(r, g, b, 0);
	}
	void Renderer::setClearColorRGBA(const float r, const float g, const float b, const float a) {
		glClearColor(r, g, b, a);
	}
	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	/// <summary>
	/// Sets OpenGL's viewport. The range of pixels to render in.
	/// </summary>
	/// <param name="width">Width of zone (window).</param>
	/// <param name="height">Height of zone (window).</param>
	/// <param name="lOffset">Left offset from (0,0).</param>
	/// <param name="bOffser">Bottom offset from (0,0).</param>
	void Renderer::setViewport(const unsigned int width, const unsigned int height,
		const unsigned int lOffset, const unsigned int bOffser) {
		glViewport(lOffset, bOffser, width, height);
	}

	const char* Renderer::getVendorStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}
	const char* Renderer::getRendererStr() {
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}
	const char* Renderer::getVersionStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

}