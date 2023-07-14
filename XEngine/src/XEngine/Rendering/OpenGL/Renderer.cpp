#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "XEngine/Log.hpp"

namespace XEngine::OpenGL {

	const char* glSource_to_String(const GLenum source){
		switch (source) {
			case GL_DEBUG_SOURCE_API:
				return "DEBUG_SOURCE_API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "DEBUG_SOURCE_WINDOW_SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "GL_DEBUG_SOURCE_SHADER_COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "DEBUG_SOURCE_THIRD_PARTY";
			case GL_DEBUG_SOURCE_APPLICATION:
				return "DEBUG_SOURCE_APPLICATION";
			case GL_DEBUG_SOURCE_OTHER:
				return "DEBUG_SOURCE_OTHER";
			default:
				return "UNKNOWN_DEBUG_SOURCE";
		}
	}

	const char* glType_to_String(const GLenum type) {
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				return "DEBUG_TYPE_ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY:
				return "DEBUG_TYPE_PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:
				return "DEBUG_TYPE_PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:
				return "DEBUG_TYPE_MARKER";
			case GL_DEBUG_TYPE_PUSH_GROUP:
				return "DEBUG_TYPE_PUSH_GROUP";
			case GL_DEBUG_TYPE_POP_GROUP:
				return "DEBUG_TYPE_POP_GROUP";
			case GL_DEBUG_TYPE_OTHER:
				return "DEBUG_TYPE_OTHER";
			default:
				return "UNKNOWN_DEBUG_TYPE";
		}
	}

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
		//Enable debug output.
#if GL_MAJOR_VERSION >= 3 && GL_MINOR_VERSION >= 4
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback([](GLenum source, GLenum type,
			GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam) {
				switch (severity) {
					case GL_DEBUG_SEVERITY_HIGH:
						LOG_ERRR("OpenGL Error: [{0}:{1}]({2}): {3}",
							glSource_to_String(source),
							glType_to_String(type),
							id,
							message);
						break;
					case GL_DEBUG_SEVERITY_MEDIUM:
						LOG_WARN("OpenGL Warning: [{0}:{1}]({2}): {3}",
							glSource_to_String(source),
							glType_to_String(type),
							id,
							message);
						break;
					case GL_DEBUG_SEVERITY_LOW:
						LOG_INFO("OpenGL Info: [{0}:{1}]({2}): {3}",
							glSource_to_String(source),
							glType_to_String(type),
							id,
							message);
						break;
					case GL_DEBUG_SEVERITY_NOTIFICATION:
						LOG_INFO("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
							glSource_to_String(source),
							glType_to_String(type),
							id,
							message);
						break;
					default:
						LOG_ERRR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
							glSource_to_String(source),
							glType_to_String(type),
							id,
							message);
				}
		}, nullptr);
#endif
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	void Renderer::enableDepthTesting() {
		glEnable(GL_DEPTH_TEST);
	}
	void Renderer::disableDepthTesting() {
		glDisable(GL_DEPTH_TEST);
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