#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "XEngine/Rendering/Renderer.hpp"
#include "XEngine/Rendering/Window.hpp"
#include "XEngine/Input/Keyboard.hpp"
#include "XEngine/Input/Mouse.hpp"

namespace XEngine {

    unsigned int Window::width = 800;
    unsigned int Window::height = 600;

    Window::Window() : m_window(nullptr) {}

    void Window::disable_cap_60() { cap_60 = false; }

    /// <summary>
    /// Callback for window movement.
    /// </summary>
    /// <param name="t_window">Window handle.</param>
    /// <param name="t_width">Window width.</param>
    /// <param name="t_height">Window height.</param>
    void Window::framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height) {
        //Set viewport.
        glViewport(0, 0, t_width, t_height);
        //Update window params.
        width = t_width;
        height = t_height;
    }

    /// <summary>
    /// Window initialization.
    /// </summary>
    /// <returns>true=success;false=failure</returns>
    bool Window::initialize() {
        //Create window.
        m_window = glfwCreateWindow(width, height, "Hello XEngine!", NULL, NULL);
        if (!m_window) { return false; }
        //Set context.
        glfwMakeContextCurrent(m_window);
#ifndef RENDER_CAP_60_SWAP
        glfwSwapInterval(0);
#endif
        return true;
    }

    /// <summary>
    /// Updates events and clears buffers.
    /// </summary>
    void Window::update() {
        //Window processes.
#ifndef RENDER_CAP_60_NDB
        glFlush();
#else
        glfwSwapBuffers(m_window);
#endif
        glfwPollEvents();
        //Set clear color.
        glm::vec4 color = Renderer::get_clear_color();
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /// <summary>
    /// Sets up all parameters.
    /// </summary>
    void Window::set_params() {
        //Set viewport.
        glViewport(0, 0, width, height);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        //Callbacks for input.
        glfwSetKeyCallback(m_window, Keyboard::key_callback);
        glfwSetCursorPosCallback(m_window, Mouse::cursor_callback);
        glfwSetMouseButtonCallback(m_window, Mouse::button_callback);
        glfwSetScrollCallback(m_window, Mouse::scroll_callback);
        //Enable depth test.
        glEnable(GL_DEPTH_TEST);
    }

    /// <summary>
    /// Set cursor state.
    /// </summary>
    /// <param name="t_state">State value.</param>
    void Window::set_cursor_state(CursorState t_state) {
        switch (t_state) {
        case NONE:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case LOCKED:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
            break;
        case DISABLED:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case HIDDEN:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
    }

    /// <summary>
    /// Closes window.
    /// </summary>
    void Window::close() {
        glfwSetWindowShouldClose(m_window, true);
    }

    /// <summary>
    /// Checks if window's going to close.
    /// </summary>
    /// <returns></returns>
    bool Window::closing() {
        return glfwWindowShouldClose(m_window) == 1;
    }

}