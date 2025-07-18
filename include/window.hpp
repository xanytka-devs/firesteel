#ifndef FS_WINDOW_H
#define FS_WINDOW_H
#include <filesystem>

#include "common.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "utils/stbi_global.hpp"

/*
    *OpenGL context*
    Maybe will be moved or removed later.
    Override by putting defines before inclusion.
*/
#ifndef FS_CONTEXT_MAJOR
#define FS_CONTEXT_MAJOR 4
#endif // !FS_CONTEXT_MAJOR
#ifndef FS_CONTEXT_MINOR
#define FS_CONTEXT_MINOR 3
#endif // !FS_CONTEXT_MINOR

namespace Firesteel {
    enum WindowState {
        // Decorated window.
        WS_NORMAL=0,
        // Undecorated fullscreen window.
        WS_FULLSCREEN=1,
        // Undecorated fullscreen-sized window.
        WS_BORDERLESS=2
    };

	struct Window {
	public:
		Window(const unsigned int& tWidth=800, const unsigned int& tHeight=600, const bool& tVsync=false) :
			mPtr(NULL), mWidth(tWidth), mHeight(tHeight), mVSync(tVsync), mClearColor(glm::vec3(0)), mClosed(false) {}

		bool initialize(const char* tTitle="Firesteel App", const WindowState& tState=WS_NORMAL) {
            //Initialize and configure.
            LOG_INFO(std::string("Creating window \"") + tTitle + "\"");
            if(glfwInit() == GLFW_FALSE) {
                LOG_CRIT("Failed to initialize GLFW");
                return false;
            }
#if _DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
#endif // _DEBUG
            glfwWindowHint(GL_FRAMEBUFFER_SRGB, GL_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FS_CONTEXT_MAJOR);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FS_CONTEXT_MINOR);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
            if(tState==WS_BORDERLESS) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwSetErrorCallback(errorCallback);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            //Window creation.
            GLFWmonitor* mon=NULL;
            if(tState==WS_FULLSCREEN || tState == WS_BORDERLESS) mon=glfwGetPrimaryMonitor();
            if(tState==WS_BORDERLESS) {
                const GLFWvidmode* vidm=glfwGetVideoMode(mon);
                mWidth=vidm->width;
                mHeight=vidm->height;
                mon=nullptr;
            }
            mPtr=glfwCreateWindow(mWidth, mHeight, tTitle, mon, NULL);
            if (mPtr == NULL) {
                LOG_CRIT("Failed to create GLFW window");
                glfwTerminate();
                return false;
            }
            glfwMakeContextCurrent(mPtr);
            //Setup vsync.
            if(mVSync) glfwSwapInterval(1);
            else glfwSwapInterval(0);
            //Setup additional parametrs.
            glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
            //Setup callbacks.
            glfwSetFramebufferSizeCallback(mPtr, framebufferSizeCallback);
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
        void close() { mClosed=true; }
        void setTitle(const std::string& tTitle) { glfwSetWindowTitle(mPtr, tTitle.c_str()); }
        void setIcon(const std::string& tIcon) {
            if(!std::filesystem::exists(tIcon)) {
                LOG_ERRR("File '" + tIcon + "' doesn't exist.");
                return;
            }
            GLFWimage images[1]{};
            images[0].pixels=stbi_load(tIcon.c_str(), &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
        }
        void setIconFromMemory(const unsigned char* tIconData, const size_t tIconDataSize) {
            GLFWimage images[1]{};
            images[0].pixels=stbi_load_from_memory(tIconData, (int)tIconDataSize, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
        }
        void setClearColor(const glm::vec3& tColor) { mClearColor=tColor; }

        void setVSync(const bool& tVSync) {
            mVSync=tVSync;
            glfwSwapInterval(mVSync);
        }
        void toggleVSync() { setVSync(!mVSync); }
        bool getVSync() const { return mVSync; }

        bool isOpen() const { return (!mClosed && !glfwWindowShouldClose(mPtr)); }
        bool isMinimized() const { return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED); }
        bool isIconified() const { return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED); }
        bool isMaximized() const { return glfwGetWindowAttrib(mPtr, GLFW_MAXIMIZED); }
        bool isFocused() const { return glfwGetWindowAttrib(mPtr, GLFW_FOCUSED); }

        void setOpacity(const float& tOpac=1.f) const { glfwSetWindowOpacity(mPtr, tOpac); }
        float getOpacity() const { return glfwGetWindowOpacity(mPtr); }
        
        void setPosition(const int& tX, const int& tY) { glfwSetWindowPos(mPtr,tX,tY); }
        glm::vec2& getPosition() {
            int x, y=0;
            glfwGetWindowPos(mPtr,&x,&y);
            return glm::vec2(x,y);
        }
        int getX() { return static_cast<int>(getPosition().x); }
        int getY() { return static_cast<int>(getPosition().y); }

        void setResizability(const bool& tResizable=true) { glfwSetWindowAttrib(mPtr,GLFW_RESIZABLE,tResizable); }
        void setSize(const int& tX, const int& tY) { glfwSetWindowSize(mPtr,tX,tY); }
        void setSizeLimit(const int& tMinX=GLFW_DONT_CARE, const int& tMinY=GLFW_DONT_CARE,
            const int& tMaxX=GLFW_DONT_CARE, const int& tMaxY=GLFW_DONT_CARE) {
                glfwSetWindowSizeLimits(mPtr,tMinX,tMinY,tMaxX,tMaxY);
        }
        int getHeight() { getSizeInternal(); return mHeight; }
        int getWidth() { getSizeInternal(); return mWidth; }
        glm::vec2 getSize() { getSizeInternal(); return glm::vec2(mWidth, mHeight); }
        float aspect() const { return static_cast<float>(mWidth) / static_cast<float>(mHeight); }

        GLFWwindow* ptr() const { return mPtr; }

        enum CursorState {
            CUR_NORMAL=0,
            CUR_CAPTURED=1,
            CUR_HIDDEN=2,
            CUR_DISABLED=3,
            CUR_UNAVAILABLE=4
        };
        void setCursorMode(const CursorState& tMode) {
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
        enum Cursor {
            //Default arrow cursor.
            CUR_ARROW=0x00036001,
            //Text editing cursor.
            CUR_IBEAM=0x00036002,
            CUR_EDIT_TEXT=CUR_IBEAM,
            CUR_CROSSHAIR=0x00036003,
            CUR_POINTING_HAND=0x00036004,
            CUR_RESIZE_H=0x00036005,
            CUR_RESIZE_V=0x00036006,
            //Top-left to bottom-right diagonal resize arrows.
            CUR_RESIZE_D_TL_BR=0x00036007,
            //Top-right to bottom-left diagonal resize arrows.
            CUR_RESIZE_D_TR_BL=0x00036008,
            //Omnidirectional resize arrows.
            CUR_RESIZE_ALL=0x00036009,
            //When the operation isn't allowed.
            CUR_NOT_ALLOWED=0x0003600A
        };
        // [!EXPIREMENTAL]
        // Wasn't tested on memory overflow. Creates standard cursors every frame.
        void setCursor(const Cursor& tCursor) {
            glfwSetCursor(mPtr,glfwCreateStandardCursor(tCursor));
        }
	private:
        glm::vec3 mClearColor;
		GLFWwindow* mPtr;
		int mWidth, mHeight;
		bool mVSync, mClosed;
    private:
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        static void errorCallback(int tEC, const char* tDescription) {
            LOGF_ERRR("GLFW Error(%i): %s\n", tEC, tDescription);
        }
        void getSizeInternal() {
            glfwGetWindowSize(mPtr, &mWidth, &mHeight);
        }
	};
}
#endif // !FS_WINDOW_H