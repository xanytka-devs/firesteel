#include <thread>
#include <mutex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "XEngine/App.hpp"
#include "XEngine/Log.hpp"
#include "XEngine/Rendering/Renderer.hpp"

namespace XEngine {

    double last_frame_fps = 0.f;
    double last_frame = 0.f;
    int frameCount = 0;

    /// <summary>
    /// Occures at app startup (instantiation).
    /// </summary>
    App::App() {
        LOG_INFO("Initializing XEngine App.");
    }

    /// <summary>
    /// Occures on app shutdown (destruction).
    /// </summary>
    App::~App() {
        LOG_INFO("Shutting down XEngine App.");
    }

    /// <summary>
    /// Quits app.
    /// </summary>
    void App::shutdown() {
        window.close();
    }

    std::mutex render_mutex;
    void update_loop(App* app, Window* window, float delta_time, double last_frame, double last_frame_fps, int fps) {
        while (!(*window).closing()) {
            //Lock mutex.
            std::lock_guard<std::mutex> lock(render_mutex);
            //Update delta time.
            double cur_time = glfwGetTime();
            delta_time = static_cast<float>(cur_time - last_frame);
            last_frame = cur_time;
            frameCount++;
            if (cur_time - last_frame_fps >= 1.0) {
                fps = frameCount;
                frameCount = 0;
                last_frame_fps = cur_time;
            }
            //Send update to recievers.
            app->update();
        }
    }

    /// <summary>
    /// Creates new instance of window for application.
    /// </summary>
    /// <param name="t_win_width">Width of the window.</param>
    /// <param name="t_win_height">Height of the window.</param>
    /// <param name="t_title">Name of the window.</param>
    /// <returns>Exit code. Only 0 is success.</returns>
    int App::start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title) {
        //Initiate core//
        Renderer::initialize();
        //Create window.
        window = Window(t_win_width, t_win_height, t_title);
        if(!window.initialize()) {
            LOG_CRIT("Failed to create window.");
            Renderer::terminate();
            return -1;
        }
        //Initialize renderer libs.
        if(!Renderer::initialize_libs()) {
            LOG_CRIT("Failed to initialize libs.");
            return -1;
        }
        //Set parameters.
        window.set_params();
        initiazile();
        std::thread render_thread(update_loop, this, &window, delta_time, last_frame, last_frame_fps, fps);
        //Update loop//
        while(!window.closing()) {
            //Enter critical section.
            window.ui_update();
            window.update();
            std::lock_guard<std::mutex> lock(render_mutex);
            window.ui_draw();
        }
        //Terminate libs and rendering//
        render_thread.join();
        on_shutdown();
        Renderer::terminate();
        return 0;
    }

}