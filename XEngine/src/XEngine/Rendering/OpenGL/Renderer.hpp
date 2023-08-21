#pragma once

struct GLFWwindow;

namespace XEngine::Rendering {
	class VertexArray;

	typedef void (*ErrorFunction)(int errorCode, const char* description);

	class Renderer {
	public:
		static bool init(GLFWwindow* pWindow);

		static void setErrorCallback(const ErrorFunction& callback);
		static bool initLibrary();
		static void setDebugContext(const bool v);

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