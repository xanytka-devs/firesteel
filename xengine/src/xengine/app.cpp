#include "xengine/app.hpp"
#include "xengine/log.hpp"
#include "xengine/enviroment.hpp"
#include "xengine/rendering/renderer.hpp"

namespace XEngine {

    double last_frame_fps = 0.f;
    double last_frame = 0.f;
    int frameCount = 0;
    App* App::m_instance{ nullptr };

    App::App() {
        m_instance = this;
        LOG_INFO("Initializing XEngine App.");
    }

    App::~App() {
        LOG_INFO("Shutting down XEngine App.");
    }

    void App::shutdown() {
        window.close();
    }

    bool drawn_ui = true;
    void App::update_loop_call() {
        //Update delta time.
        double cur_time = Renderer::get_time();
        Enviroment::delta_time = static_cast<float>(cur_time - last_frame);
        last_frame = cur_time;
        frameCount++;
        if(cur_time - last_frame_fps >= 1.0) {
            fps = frameCount;
            frameCount = 0;
            last_frame_fps = cur_time;
        }
        //Send update to recievers.
        if(!drawn_ui) { window.ui_draw(); drawn_ui = true; }
        drawn_ui = false;
        window.ui_update();
        window.update();
        if(update_app) update();
    }

    int App::start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title) {
        //Initiate core//
        preinitiazile();
        Renderer::initialize();
        //Create window.
        window = Window(t_win_width, t_win_height, t_title);
        if(!window.initialize()) {
            LOG_CRIT("Failed to create window.");
            Renderer::terminate();
            return -1;
        }
        //Initialize renderer libs.
        if(!Renderer::initialize_libs(window.get_handle())) {
            LOG_CRIT("Failed to initialize libs.");
            return -1;
        }
        //Set parameters.
        window.set_init_params();
        initiazile();
        update_loop_call();
        //Update loop//
        while(!window.closing()) {
            update_loop_call();
            update_app = true;
        }
        //Terminate libs and rendering//
        on_shutdown();
        Renderer::terminate();
        return 0;
    }

}