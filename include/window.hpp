#ifndef FS_WINDOW_H
#define FS_WINDOW_H
#include "common.hpp"
#include <filesystem>
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

	class Window {
	public:
		Window(const unsigned int& tWidth = 800, const unsigned int& tHeight = 600, const bool& tVsync = false) :
			mPtr(NULL), mWidth(tWidth), mHeight(tHeight), mVSync(tVsync), mClearColor(glm::vec3(0)), mClosed(false) {}

		bool initialize(const char* tTitle = "Firesteel App", const WindowState& tState = WS_NORMAL,
            size_t tContextMajor = 3, size_t tContextMinor = 3) {
            // Initialize and configure.
            LOG_INFO(std::string("Creating window \"") + tTitle + "\"");
            if(glfwInit() == GLFW_FALSE) return false;
#if _DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
#endif // _DEBUG
            glfwWindowHint(GL_FRAMEBUFFER_SRGB, GL_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(tContextMajor));
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(tContextMinor));
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
            if(tState==WS_BORDERLESS) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwSetErrorCallback(errorCallback);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            // Window creation.
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
            // Setup vsync.
            if(mVSync) glfwSwapInterval(1);
            else glfwSwapInterval(0);
            // Setup parametrs.
            glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
            // Setup callbacks.
            glfwSetFramebufferSizeCallback(mPtr, framebufferSizeCallback);
            glfwSetCursorPosCallback(mPtr, Mouse::cursorCallback);
            glfwSetMouseButtonCallback(mPtr, Mouse::buttonCallback);
            glfwSetScrollCallback(mPtr, Mouse::scrollCallback);
            glfwSetKeyCallback(mPtr, Keyboard::keyCallback);
            return true;
		}
        void swapBuffers() const {
            glfwSwapBuffers(mPtr);
        }
        void pollEvents() const {
            glfwPollEvents();
        }
        void clearBuffers() const {
            glClearColor(static_cast<GLfloat>(mClearColor[0]), static_cast<GLfloat>(mClearColor[1]),
                static_cast<GLfloat>(mClearColor[2]), static_cast<GLfloat>(1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
        void close() {
            mClosed = true;
        }
        void terminate() const {
            glfwTerminate();
        }
        void setTitle(const std::string& tTitle) {
            glfwSetWindowTitle(mPtr, tTitle.c_str());
        }
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
        void setIconFromMemory(unsigned char* tIconData, const size_t tIconDataSize) {
            GLFWimage images[1]{};
            images[0].pixels = stbi_load_from_memory(tIconData, (int)tIconDataSize, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
        }
        void setClearColor(const glm::vec3& tColor) {
            mClearColor = tColor;
        }
        void setVSync(const bool& tVSync) {
            mVSync = tVSync;
            if (mVSync) glfwSwapInterval(1);
            else glfwSwapInterval(0);
        }
        void toggleVSync() { setVSync(!mVSync); }
        bool getVSync() const { return mVSync; }

        bool isOpen() const { return (!mClosed && !glfwWindowShouldClose(mPtr)); }
        int getHeight() { getSizeInternal(); return mHeight; }
        int getWidth() { getSizeInternal(); return mWidth; }
        glm::vec2 getSize() { getSizeInternal(); return glm::vec2(mWidth, mHeight); }
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
	private:
		GLFWwindow* mPtr;
		int mWidth, mHeight;
		bool mVSync, mClosed;
		glm::vec3 mClearColor;
    private:
        //Resize viewport.
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        static void errorCallback(int tEC, const char* tDescription) {
            LOG_ERRR("GLFW Error(" + std::to_string(tEC) + "): " + tDescription);
        }
        void getSizeInternal() {
            glfwGetWindowSize(mPtr, &mWidth, &mHeight);
        }
	};
}
#endif // !FS_WINDOW_H