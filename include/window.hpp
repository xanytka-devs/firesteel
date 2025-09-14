#ifndef FS_WINDOW_H
#define FS_WINDOW_H
#include <filesystem>

#include "enviroment.hpp"
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

#ifdef FS_HEADLESS
typedef struct GLFWwindow {};
#endif // !FS_HEADLESS

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
#ifndef FS_HEADLESS
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
            glfwSetErrorCallback(_errorCallback);
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
            glfwSetFramebufferSizeCallback(mPtr, _framebufferSizeCallback);
            glfwSetCursorPosCallback(mPtr, Mouse::cursorCallback);
            glfwSetMouseButtonCallback(mPtr, Mouse::buttonCallback);
            glfwSetScrollCallback(mPtr, Mouse::scrollCallback);
            glfwSetKeyCallback(mPtr, Keyboard::keyCallback);
#endif // !FS_HEADLESS
            return true;
		}
        void swapBuffers() const {
#ifndef FS_HEADLESS
            glfwSwapBuffers(mPtr);
#endif // !FS_HEADLESS
        }
        void pollEvents() const {
#ifndef FS_HEADLESS
            glfwPollEvents();
#endif // !FS_HEADLESS
        }
        void clearBuffers() const {
#ifndef FS_HEADLESS
        glClearColor(static_cast<GLfloat>(mClearColor[0]), static_cast<GLfloat>(mClearColor[1]),
        static_cast<GLfloat>(mClearColor[2]), static_cast<GLfloat>(1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif // !FS_HEADLESS
        }
        void close() { mClosed=true; }
        void setTitle(const std::string& tTitle) {
#ifndef FS_HEADLESS
            glfwSetWindowTitle(mPtr, tTitle.c_str());
#endif // !FS_HEADLESS
        }
        void setIcon(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_ERRR("File '" + tPath + "' doesn't exist.");
                return;
            }
#ifndef FS_HEADLESS
            GLFWimage images[1]{};
            images[0].pixels=stbi_load(tPath.c_str(), &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
#endif // !FS_HEADLESS
        }
        void setIconFromMemory(const unsigned char* tIconData, const size_t tIconDataSize) {
#ifndef FS_HEADLESS
            GLFWimage images[1]{};
            images[0].pixels=stbi_load_from_memory(tIconData, (int)tIconDataSize, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(mPtr, 1, images);
            stbi_image_free(images[0].pixels);
#endif // !FS_HEADLESS
        }
        void setClearColor(const glm::vec3& tColor) { mClearColor=tColor; }

        void setVSync(const bool& tVSync) {
            mVSync=tVSync;
#ifndef FS_HEADLESS
            glfwSwapInterval(mVSync);
#endif // !FS_HEADLESS
        }
        void toggleVSync() { setVSync(!mVSync); }
        bool getVSync() const { return mVSync; }

        bool isOpen() const {
            return (!mClosed
#ifndef FS_HEADLESS
            && !glfwWindowShouldClose(mPtr)
#endif // !FS_HEADLESS
            );
        }
        bool isMinimized() const {
#ifndef FS_HEADLESS
            return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED);
#endif // !FS_HEADLESS
            return true;
        }
        bool isIconified() const {
#ifndef FS_HEADLESS
            return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED);
#endif // !FS_HEADLESS
            return true;
        }
        bool isMaximized() const {
#ifndef FS_HEADLESS
            return glfwGetWindowAttrib(mPtr, GLFW_MAXIMIZED);
#endif // !FS_HEADLESS
            return false;
        }
        bool isFocused() const {
#ifndef FS_HEADLESS
            return glfwGetWindowAttrib(mPtr, GLFW_FOCUSED);
#endif // !FS_HEADLESS
            return false;
        }

        void setOpacity(const float& tOpac=1.f) const {
#ifndef FS_HEADLESS
            glfwSetWindowOpacity(mPtr, tOpac);
#endif // !FS_HEADLESS
        }
        float getOpacity() const {
#ifndef FS_HEADLESS
            return glfwGetWindowOpacity(mPtr);
#endif // !FS_HEADLESS
            return 1;
        }
        
        void setPosition(const int& tX, const int& tY) {
#ifndef FS_HEADLESS
            glfwSetWindowPos(mPtr,tX,tY);
#endif // !FS_HEADLESS
        }
        glm::vec2& getPosition() {
            int x, y=0;
#ifndef FS_HEADLESS
            glfwGetWindowPos(mPtr,&x,&y);
#endif // !FS_HEADLESS
            return glm::vec2(x,y);
        }
        int getX() { return static_cast<int>(getPosition().x); }
        int getY() { return static_cast<int>(getPosition().y); }

        void setResizability(const bool& tResizable=true) {
#ifndef FS_HEADLESS
            glfwSetWindowAttrib(mPtr,GLFW_RESIZABLE,tResizable);
#endif // !FS_HEADLESS
        }
        void setSize(const int& tX, const int& tY) {
#ifndef FS_HEADLESS
            glfwSetWindowSize(mPtr,tX,tY);
#endif // !FS_HEADLESS
        }
        void setSizeLimit(const int& tMinX=GLFW_DONT_CARE, const int& tMinY=GLFW_DONT_CARE,
            const int& tMaxX=GLFW_DONT_CARE, const int& tMaxY=GLFW_DONT_CARE) {
#ifndef FS_HEADLESS
                glfwSetWindowSizeLimits(mPtr,tMinX,tMinY,tMaxX,tMaxY);
#endif // !FS_HEADLESS
        }
        int getHeight() { _getSizeInternal(); return mHeight; }
        int getWidth() { _getSizeInternal(); return mWidth; }
        glm::vec2 getSize() { _getSizeInternal(); return glm::vec2(mWidth, mHeight); }
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
#ifndef FS_HEADLESS
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
#endif // !FS_HEADLESS
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
#ifndef FS_HEADLESS
            glfwSetCursor(mPtr,glfwCreateStandardCursor(tCursor));
#endif // !FS_HEADLESS
        }
	private:
        glm::vec3 mClearColor;
		GLFWwindow* mPtr;
		int mWidth, mHeight;
		bool mVSync, mClosed;
    private:
        static void _framebufferSizeCallback(GLFWwindow* window, int width, int height) {
#ifndef FS_HEADLESS
            glViewport(0, 0, width, height);
#endif // !FS_HEADLESS
        }
        static void _errorCallback(int tEC, const char* tDescription) {
            LOGF_ERRR("GLFW Error(%i): %s\n", tEC, tDescription);
        }
        void _getSizeInternal() {
#ifndef FS_HEADLESS
            glfwGetWindowSize(mPtr, &mWidth, &mHeight);
#endif // !FS_HEADLESS
        }
	};
}
#endif // !FS_WINDOW_H