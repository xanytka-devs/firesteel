#include <firesteel/window.hpp>

#include <filesystem>

#include <firesteel/enviroment.hpp>
#include <firesteel/input/keyboard.hpp>
#include <firesteel/input/mouse.hpp>
#include <firesteel/util/stbi_global.hpp>

namespace Firesteel {
    static void _framebufferSizeCallback(GLFWwindow* window, int width, int height) {
#ifndef FS_HEADLESS
        Enviroment::sInstance()->renderer->setViewportSize(width, height);
#endif // !FS_HEADLESS
    }
    static void _errorCallback(int tEC, const char* tDescription) {
        LOGF_ERRR("GLFW Error(%i): %s\n", tEC, tDescription);
    }
    static void _getSizeInternal(GLFWwindow* window, int& width, int& height) {
#ifndef FS_HEADLESS
        glfwGetWindowSize(window, &width, &height);
#endif // !FS_HEADLESS
    }


    Window::Window(const unsigned int& tWidth, const unsigned int& tHeight, const bool& tVsync) :
        mPtr(NULL), mWidth(tWidth), mHeight(tHeight), mVSync(tVsync), mClearColor(glm::vec3(0)), mClosed(false) {
    }

    bool Window::initialize(const char* tTitle, const WindowState& tState) {
        //Initialize and configure.
        LOGF_INFO("Creating window \"%s\"", tTitle);
#ifndef FS_HEADLESS
        if (glfwInit() == GLFW_FALSE) {
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
        if (tState == WS_BORDERLESS) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwSetErrorCallback(_errorCallback);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        //Window creation.
        GLFWmonitor* mon = NULL;
        if (tState == WS_FULLSCREEN || tState == WS_BORDERLESS) mon = glfwGetPrimaryMonitor();
        if (tState == WS_BORDERLESS) {
        const GLFWvidmode* vidm = glfwGetVideoMode(mon);
        mWidth = vidm->width;
        mHeight = vidm->height;
        mon = nullptr;
        }
        mPtr = glfwCreateWindow(mWidth, mHeight, tTitle, mon, NULL);
        if (mPtr == NULL) {
        LOG_CRIT("Failed to create GLFW window");
        glfwTerminate();
        return false;
        }
        glfwMakeContextCurrent(mPtr);
        //Setup vsync.
        if (mVSync) glfwSwapInterval(1);
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
    void Window::swapBuffers() const {
#ifndef FS_HEADLESS
        glfwSwapBuffers(mPtr);
#endif // !FS_HEADLESS
    }
    void Window::pollEvents() const {
#ifndef FS_HEADLESS
        glfwPollEvents();
#endif // !FS_HEADLESS
    }
    void Window::clearBuffers() const {
#ifndef FS_HEADLESS
        Enviroment::sInstance()->renderer->clearBuffers(mClearColor);
#endif // !FS_HEADLESS
    }
    void Window::setTitle(const std::string& tTitle) {
#ifndef FS_HEADLESS
        glfwSetWindowTitle(mPtr, tTitle.c_str());
#endif // !FS_HEADLESS
    }
    void Window::setIcon(const std::string& tPath) {
        if (!std::filesystem::exists(tPath)) {
            LOG_ERRR("Icon at \"" + tPath + "\" doesn't exist.");
            return;
        }
#ifndef FS_HEADLESS
        GLFWimage images[1]{};
        images[0].pixels = stbi_load(tPath.c_str(), &images[0].width, &images[0].height, 0, 4);
        glfwSetWindowIcon(mPtr, 1, images);
        stbi_image_free(images[0].pixels);
#endif // !FS_HEADLESS
    }
    void Window::setIconFromMemory(const unsigned char* tIconData, const size_t tIconDataSize) {
#ifndef FS_HEADLESS
        GLFWimage images[1]{};
        images[0].pixels = stbi_load_from_memory(tIconData, (int)tIconDataSize, &images[0].width, &images[0].height, 0, 4);
        glfwSetWindowIcon(mPtr, 1, images);
        stbi_image_free(images[0].pixels);
#endif // !FS_HEADLESS
    }

    void Window::setVSync(const bool& tVSync) {
        mVSync = tVSync;
#ifndef FS_HEADLESS
        glfwSwapInterval(mVSync);
#endif // !FS_HEADLESS
    }

    bool Window::isOpen() const {
        return (!mClosed
#ifndef FS_HEADLESS
        && !glfwWindowShouldClose(mPtr)
#endif // !FS_HEADLESS
        );
    }
    bool Window::isMinimized() const {
#ifndef FS_HEADLESS
        return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED);
#endif // !FS_HEADLESS
        return true;
    }
    bool Window::isIconified() const {
#ifndef FS_HEADLESS
        return glfwGetWindowAttrib(mPtr, GLFW_ICONIFIED);
#endif // !FS_HEADLESS
        return true;
    }
    bool Window::isMaximized() const {
#ifndef FS_HEADLESS
        return glfwGetWindowAttrib(mPtr, GLFW_MAXIMIZED);
#endif // !FS_HEADLESS
        return false;
    }
    bool Window::isFocused() const {
#ifndef FS_HEADLESS
        return glfwGetWindowAttrib(mPtr, GLFW_FOCUSED);
#endif // !FS_HEADLESS
        return false;
    }

    void Window::setOpacity(const float& tOpac) const {
#ifndef FS_HEADLESS
        glfwSetWindowOpacity(mPtr, tOpac);
#endif // !FS_HEADLESS
    }
    float Window::getOpacity() const {
#ifndef FS_HEADLESS
        return glfwGetWindowOpacity(mPtr);
#endif // !FS_HEADLESS
        return 1;
    }

    void Window::setPosition(const int& tX, const int& tY) {
#ifndef FS_HEADLESS
        glfwSetWindowPos(mPtr, tX, tY);
#endif // !FS_HEADLESS
    }
    glm::vec2 Window::getPosition() {
        int x, y = 0;
#ifndef FS_HEADLESS
        glfwGetWindowPos(mPtr, &x, &y);
#endif // !FS_HEADLESS
        return glm::vec2(x, y);
    }

    void Window::setResizability(const bool& tResizable) {
#ifndef FS_HEADLESS
        glfwSetWindowAttrib(mPtr, GLFW_RESIZABLE, tResizable);
#endif // !FS_HEADLESS
    }
    void Window::setSize(const int& tX, const int& tY) {
#ifndef FS_HEADLESS
        glfwSetWindowSize(mPtr, tX, tY);
#endif // !FS_HEADLESS
    }
    void Window::setSizeLimit(const int& tMinX, const int& tMinY ,
        const int& tMaxX, const int& tMaxY) {
#ifndef FS_HEADLESS
        glfwSetWindowSizeLimits(mPtr, tMinX, tMinY, tMaxX, tMaxY);
#endif // !FS_HEADLESS
    }
    int Window::getHeight() { _getSizeInternal(mPtr, mWidth, mHeight); return mHeight; }
    int Window::getWidth() { _getSizeInternal(mPtr, mWidth, mHeight); return mWidth; }
    glm::vec2 Window::getSize() { _getSizeInternal(mPtr, mWidth, mHeight); return glm::vec2(mWidth, mHeight); }
    float Window::aspect() const { return static_cast<float>(mWidth) / static_cast<float>(mHeight); }

    GLFWwindow* Window::ptr() const { return mPtr; }

    void Window::setCursorMode(const CursorState& tMode) {
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

    void Window::setCursor(const Cursor& tCursor) {
#ifndef FS_HEADLESS
        glfwSetCursor(mPtr, glfwCreateStandardCursor(tCursor));
#endif // !FS_HEADLESS
    }
}