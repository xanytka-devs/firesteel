#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include "XEngine/Event.hpp"
#include "XEngine/Window.hpp"
#include "XEngine/Log.hpp"
#include "XEngine/UI/TUI.hpp"
#include "XEngine/Rendering/OpenGL/Renderer.hpp"
#include "XEngine/Camera.hpp"

namespace XEngine {

    using namespace XEngine::OpenGL;
    using namespace XEngine::UI;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: w_data({std::move(title), width, height}) {
        //Initialize basis.
		int result = initialize();
	}
	Window::~Window() {
		shutdown();
	}

    int Window::initialize() {

        LOG_INFO("Creating instance of window '{0}', with size of {1}x{2}.", w_data.title, w_data.width, w_data.height);

        glfwSetErrorCallback([](int errorCode, const char* description) { LOG_CRIT("GLFW Error: {}", description); });

        //Initialize the library.
        if (!glfwInit()) {
            LOG_CRIT("Error while trying to initialize GLFW (OpenGL).");
            return -100;
        }

        //Create a windowed mode window and its OpenGL context.
        window = glfwCreateWindow(w_data.width, w_data.height, w_data.title.c_str(), nullptr, nullptr);
        if (!window) {
            LOG_CRIT("Error while trying to initialize window '{0}'.", w_data.title);
            return -101;
        }

        //Make the window's context current.
        if (!Renderer::init(window)) {
            LOG_CRIT("Failed to initialize renderer ({0}}).", Renderer::getRendererStr());
            return -102;
        }

        //Handle events.
        glfwSetWindowUserPointer(window, &w_data);
        glfwSetKeyCallback(window, [](GLFWwindow* pWindow, int key, int scanCode, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            switch (action) {
                case GLFW_PRESS: {
                    EventKeyDown event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventKeyUp event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT: {
                    EventKeyDown event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFn(event);
                    break;
                }
            }
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow* pWindow, int button, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            double xPos;
            double yPos;
            glfwGetCursorPos(pWindow, &xPos, &yPos);
            switch (action) {
                case GLFW_PRESS: {
                    EventMouseButtonDown event(static_cast<MouseButton>(button), xPos, yPos);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventMouseButtonUp event(static_cast<MouseButton>(button), xPos, yPos);
                    data.eventCallbackFn(event);
                    break;
                }
            }
        });
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
            LOG_INFO("Window '{0}' closed.", data.title);
        });
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* pWindow, int width, int height) {
            Renderer::setViewport(width, height);
        });
        //Initialize ImGui.
        TUI::init(window);
        
        return 0;

	}
	void Window::shutdown() {
        TUI::shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
	}
	void Window::update() {
        //Swap front and back buffers.
        glfwSwapBuffers(window);
        //Poll for and process events.
        glfwPollEvents();
	}

    glm::vec2 Window::getMousePos() const {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return glm::vec2(xPos, yPos);
    }

}