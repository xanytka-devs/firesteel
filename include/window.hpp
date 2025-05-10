#ifndef FS_WINDOW_H
#define FS_WINDOW_H
#include <filesystem>

#include "common.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "utils/stbi_global.hpp"

namespace Firesteel {

    enum WindowState {
        // Decorated window.
        WS_NORMAL = 0x0,
        // Undecorated fullscreen window.
        WS_FULLSCREEN = 0x1,
        // Undecorated fullscreen-sized window.
        WS_BORDERLESS = 0x2
    };

	struct Window {
	public:
		Window(const unsigned int& tWidth = 800, const unsigned int& tHeight = 600, const bool& tVsync = false) :
			mPtr(NULL), mWidth(tWidth), mHeight(tHeight), mVSync(tVsync), mClearColor(glm::vec3(0)), mClosed(false) {}

        // Creates new window from given parametrs.
		bool initialize(const char* tTitle = "Firesteel App", const WindowState& tState = WS_NORMAL) {
            //Initialize and configure.
            LOG_INFO(std::string("Creating window \"") + tTitle + "\"");
            if(glfwInit() == GLFW_FALSE) return false;
#if _DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
#endif // _DEBUG
            glfwWindowHint(GL_FRAMEBUFFER_SRGB, GL_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
            if(tState==WS_BORDERLESS) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwSetErrorCallback(_errorCallback);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            //Window creation.
            GLFWmonitor* mon = NULL;
            if(tState==WS_FULLSCREEN || tState == WS_BORDERLESS) mon = glfwGetPrimaryMonitor();
            if(tState==WS_BORDERLESS) {
                const GLFWvidmode* vidm = glfwGetVideoMode(mon);
                mWidth = vidm->width;
                mHeight = vidm->height;
                mon = nullptr;
            }
            mPtr = glfwCreateWindow(mWidth, mHeight, tTitle, mon, NULL);
            if (mPtr == NULL) {
                LOG_ERRR("Failed to create GLFW window");
                glfwTerminate();
                return false;
            }
            glfwMakeContextCurrent(mPtr);
            //Setup vsync.
            if(mVSync) glfwSwapInterval(1);
            else glfwSwapInterval(0);
            //Setup parametrs.
            glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
            //Setup callbacks.
            glfwSetFramebufferSizeCallback(mPtr, _framebufferSizeCallback);
            glfwSetCursorPosCallback(mPtr, Mouse::cursorCallback);
            glfwSetMouseButtonCallback(mPtr, Mouse::buttonCallback);
            glfwSetScrollCallback(mPtr, Mouse::scrollCallback);
            glfwSetKeyCallback(mPtr, Keyboard::keyCallback);
            return true;
		}
        void swapBuffers() const { glfwSwapBuffers(mPtr); }
        void pollEvents() const { glfwPollEvents(); }
        void clearBuffers() const {
            glClearColor(static_cast<GLfloat>(mClearColor[0]), static_cast<GLfloat>(mClearColor[1]),
                static_cast<GLfloat>(mClearColor[2]), static_cast<GLfloat>(1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
        void close() { mClosed = true; }

        void setTitle(const std::string& tTitle) { glfwSetWindowTitle(mPtr, tTitle.c_str()); }
        void setIcon(const std::string& tIcon) {
            GLFWimage images[1]{};
            if(!std::filesystem::exists(tIcon)) {
                LOG_ERRR("File '" + tIcon + "' doesn't exist.");
                return;
            }
            images[0].pixels = stbi_load(tIcon.c_str(), &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
        }
        void setIconFromMemory(const unsigned char* tIconData, const size_t tIconDataSize) {
            GLFWimage images[1]{};
            images[0].pixels = stbi_load_from_memory(tIconData, (int)tIconDataSize, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
        }
        void setClearColor(const glm::vec3& tColor) { mClearColor = tColor; }
        
        // Set custom swap interval for buffer.
        // (0 for no VSyns, 1 for VSync, other will (probably) limit the framerate)
        void setSwapInterval(const int& tInterval) {
            if(tInterval==0) mVSync=false;
            if(tInterval==1) mVSync=true;
            glfwSwapInterval(tInterval);
        }
        void setVSync(const bool& tVSync) { setSwapInterval(tVSync); }
        void toggleVSync() { setVSync(!mVSync); }
        bool getVSync() const { return mVSync; }

        // aka window.isMinimized()
        bool isIconified() const { return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED); }
        bool isMinimized() const { return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED); }
        bool isMaximized() const { return glfwGetWindowAttrib(mPtr, GLFW_MAXIMIZED); }
        bool isFocused() const { return glfwGetWindowAttrib(mPtr, GLFW_FOCUSED); }
        bool isOpen() const { return (!mClosed && !glfwWindowShouldClose(mPtr)); }

        void setOpacity(float tOpac=1.f) const { glfwSetWindowOpacity(mPtr, tOpac); }
        float getOpacity() const { return glfwGetWindowOpacity(mPtr); }

        // Sets limits on window size.
        void setSizeConstrains(unsigned int tMinW=GLFW_DONT_CARE, unsigned int tMinH=GLFW_DONT_CARE,
            unsigned int tMaxW=GLFW_DONT_CARE, unsigned int tMaxH=GLFW_DONT_CARE) {
            glfwSetWindowSizeLimits(mPtr, tMinW, tMinH, tMaxW, tMaxH);
        }
        // Sets limits on window size.
        void setSizeConstrains(glm::vec2 tMinSize, glm::vec2 tMaxSize) {
            setSizeConstrains(tMinSize.x, tMinSize.y, tMaxSize.x, tMaxSize.y);
        }
        // Sets minimal window size, but removes it's maximum.
        void setMinimalSize(unsigned int tW, unsigned int tH) {
            setSizeConstrains(tW, tH);
        }
        // Sets minimal window size, but removes it's maximum.
        void setMinimalSize(glm::vec2 tSize) {
            setSizeConstrains(tSize.x, tSize.y);
        }
        // Sets maximal window size, but removes it's minimum.
        void setMaximalSize(unsigned int tW, unsigned int tH) {
            setSizeConstrains(GLFW_DONT_CARE, GLFW_DONT_CARE, tW, tH);
        }
        // Sets maximal window size, but removes it's minimum.
        void setMaximalSize(glm::vec2 tSize) {
            setSizeConstrains(GLFW_DONT_CARE, GLFW_DONT_CARE, tSize.x, tSize.y);
        }

        int getHeight() { _getSize(); return mHeight; }
        int getWidth() { _getSize(); return mWidth; }
        glm::vec2 getSize() { _getSize(); return glm::vec2(mWidth, mHeight); }
        float aspect() const { return static_cast<float>(mWidth) / static_cast<float>(mHeight); }

        GLFWwindow* ptr() const { return mPtr; }

        enum Cursor {
            CUR_NORMAL = 0x0,
            CUR_CAPTURED = 0x1,
            CUR_HIDDEN = 0x2,
            CUR_DISABLED = 0x3,
            CUR_UNAVAILABLE = 0x4
        };
        void setCursorMode(const Cursor& tMode) {
            switch (tMode) {
            case Window::CUR_CAPTURED:
                glfwSetInputMode(mPtr, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
                break;
            case Window::CUR_HIDDEN:
                glfwSetInputMode(mPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            case Window::CUR_DISABLED:
                glfwSetInputMode(mPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
            case Window::CUR_UNAVAILABLE:
                glfwSetInputMode(mPtr, GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);
                break;
            default:
                glfwSetInputMode(mPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            }
        }
        void setResizeablitiy(bool tVal) {
            glfwSetWindowAttrib(mPtr, GLFW_RESIZABLE, tVal?GLFW_TRUE:GLFW_FALSE);
        }
	private:
		GLFWwindow* mPtr;
		int mWidth, mHeight;
		bool mVSync, mClosed;
		glm::vec3 mClearColor;
    private:
        static void _framebufferSizeCallback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        static void _errorCallback(int tEC, const char* tDescription) {
            LOG_ERRR("GLFW Error(" + std::to_string(tEC) + "): " + tDescription);
        }
        void _getSize() {
            glfwGetWindowSize(mPtr, &mWidth, &mHeight);
        }
	};
}
#endif // !FS_WINDOW_H