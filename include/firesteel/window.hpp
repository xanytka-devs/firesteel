#ifndef FS_WINDOW_H
#define FS_WINDOW_H

#include <firesteel/common.hpp>
#include <glm/glm.hpp>

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

#define GLFW_DONT_CARE -1

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
        Window(const unsigned int& tWidth = 800, const unsigned int& tHeight = 600, const bool& tVsync = false);

        bool initialize(const char* tTitle = "Firesteel App", const WindowState& tState = WS_NORMAL);
        void swapBuffers() const;
        void pollEvents() const;
        void clearBuffers() const;
        void close() { mClosed=true; }
        void setTitle(const std::string& tTitle);
        void setIcon(const std::string& tPath);
        void setIconFromMemory(const unsigned char* tIconData, const size_t tIconDataSize);
        glm::vec3 getClearColor() const { return mClearColor; }
        void setClearColor(const glm::vec3& tColor) { mClearColor=tColor; }

        void setVSync(const bool& tVSync);
        void toggleVSync() { setVSync(!mVSync); }
        bool getVSync() const { return mVSync; }

        bool isOpen() const;
        bool isMinimized() const;
        bool isIconified() const;
        bool isMaximized() const;
        bool isFocused() const;

        void setOpacity(const float& tOpac = 1.f) const;
        float getOpacity() const;
        
        void setPosition(const int& tX, const int& tY);
        glm::vec2 getPosition();
        int getX() { return static_cast<int>(getPosition().x); }
        int getY() { return static_cast<int>(getPosition().y); }

        void setResizability(const bool& tResizable = true);
        void setSize(const int& tX, const int& tY);
        void setSizeLimit(const int& tMinX = GLFW_DONT_CARE, const int& tMinY = GLFW_DONT_CARE,
            const int& tMaxX = GLFW_DONT_CARE, const int& tMaxY = GLFW_DONT_CARE);
        int getHeight();
        int getWidth();
        glm::vec2 getSize();
        float aspect() const;

        GLFWwindow* ptr() const;

        enum CursorState {
            CUR_NORMAL=0,
            CUR_CAPTURED=1,
            CUR_HIDDEN=2,
            CUR_DISABLED=3,
            CUR_UNAVAILABLE=4
        };
        void setCursorMode(const CursorState& tMode);
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
        void setCursor(const Cursor& tCursor);
	private:
        GLFWwindow* mPtr=nullptr;
        glm::vec3 mClearColor;
		int mWidth=0, mHeight=0;
		bool mVSync, mClosed;
	};
}
#endif // !FS_WINDOW_H