#pragma once

struct GLFWwindow;

namespace XEngine::OpenGL {
	class VertexArray;

	class Renderer {
	public:
		static bool init(GLFWwindow* pWindow);

		static void draw(const VertexArray& vertexArray);
		static void setClearColorRGB(const float r, const float g, const float b);
		static void setClearColorRGBA(const float r, const float g, const float b, const float a);
		static void clear();
		static void setViewport(const unsigned int width, const unsigned int height,
			const unsigned int lOffset = 0, const unsigned int bOffser = 0);
		static void enableDepthTesting();
		static void disableDepthTesting();

		static const char* getVendorStr();
		static const char* getRendererStr();
		static const char* getVersionStr();
	};

}