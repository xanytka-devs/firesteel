#include "xengine/app.hpp"
#include "xengine/log.hpp"
#include "xengine/enviroment.hpp"
#include "xengine/rendering/renderer.hpp"

namespace XEngine {

    double last_frame_fps = 0.f;
    double last_frame = 0.f;
    int frame_count = 0;
    App* App::m_instance{ nullptr };
    unsigned char Enviroment::scene_id = 0;
    SceneManager Enviroment::scene_manager = SceneManager();

    App::App() {
        nvtx3::mark("xengine_initalize");
        m_instance = this;
        LOG_INFO("Initializing XEngine App.");
    }

    App::~App() {
        nvtx3::mark("xengine_shutdown");
        LOG_INFO("Shutting down XEngine App.");
    }

    void App::shutdown() {
        window.close();
    }

    bool drawn_ui = true;
    void App::update_loop_call() {
        nvtx3::scoped_range loop{ "update_loop_call" };
        //Update delta time.
        double cur_time = Renderer::get_time();
        Enviroment::delta_time = static_cast<float>(cur_time - last_frame);
        last_frame = cur_time;
        frame_count++;
        if(cur_time - last_frame_fps >= 1.0) {
            fps = frame_count;
            frame_count = 0;
            last_frame_fps = cur_time;
        }
        //Send update to recievers.
        if(!drawn_ui) { window.gui_draw(); drawn_ui = true; }
        window.pull_events();
        if(drawn_ui) window.gui_update();
        window.update();
        if(update_app) update();
        drawn_ui = false;
    }

    int App::start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title) {
        //Initiate core//
        preinitialize();
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
        Enviroment::scene_manager.add_scene(Scene());
        Enviroment::scene_manager.initialized = false;
        initialize();
        update_loop_call();
        //Update loop//
        nvtx3::scoped_range r{ "update_app_function" };
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