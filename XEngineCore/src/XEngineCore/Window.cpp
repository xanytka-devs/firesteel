#include "XEngineCore/Window.hpp"
#include "XEngineCore/Log.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "XEngineCore/Event.hpp"

namespace XEngine {

    static bool glfwInitialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: w_data({std::move(title), width, height}) {
		int result = initialize();
	}
	Window::~Window() {
		shutdown();
	}

    int Window:: initialize() {

        LOG_INFO("Creating instance of window '{0}', with size of {1}x{2}.", w_data.title, w_data.width, w_data.height);

        //Initialize the library.
        if (!glfwInitialized) {
            if (!glfwInit()) {
                LOG_CRIT("Error while trying to initialize GLFW (OpenGL).");
                return -100;
            }
            glfwInitialized = true;
        }

        //Create a windowed mode window and its OpenGL context.
        window = glfwCreateWindow(w_data.width, w_data.height, w_data.title.c_str(), nullptr, nullptr);
        if (!window) {
            LOG_CRIT("Error while trying to initialize window '{0}'.", w_data.title);
            glfwTerminate();
            return -101;
        }

        //Make the window's context current.
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD (OpenGL).");
            return -102;
        }

        //Handel events.
        glfwSetWindowUserPointer(window, &w_data);
        glfwSetWindowSizeCallback(window, [](GLFWwindow* pWindow, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.width = width;
            data.height = height;
            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* pWindow, double x, double y) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventMouseMove event(x, y);
            data.eventCallbackFn(event);
        });
        glfwSetWindowCloseCallback(window, [](GLFWwindow* pWindow) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventWindowClose event;
            data.eventCallbackFn(event);
        });

        return 0;

	}
	void Window::shutdown() {
        glfwDestroyWindow(window);
        glfwTerminate();
	}
	void Window::update() {

        //Clear color buffer.
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        //Swap front and back buffers.
        glfwSwapBuffers(window);
        //Poll for and process events.
        glfwPollEvents();

	}

}