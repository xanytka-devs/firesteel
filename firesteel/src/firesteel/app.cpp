#include "firesteel/app.hpp"
#include "firesteel/log.hpp"
#include "firesteel/enviroment.hpp"
#include "firesteel/rendering/renderer.hpp"

namespace firesteel {

    double last_frame_fps = 0.f;
    double last_frame = 0.f;
    int frame_count = 0;
    App* App::m_instance{ nullptr };
    unsigned char Enviroment::scene_id = 0;
    SceneManager Enviroment::scene_manager = SceneManager();

    App::App() {
#ifdef XE_NTVX3
        nvtx3::mark("firesteel_initalize");
#endif // XE_NTVX3
        m_instance = this;
        LOG_INFO("Initializing Firesteel App.");
    }

    App::~App() {
#ifdef XE_NTVX3
        nvtx3::mark("firesteel_shutdown");
#endif // XE_NTVX3
        LOG_INFO("Shutting down Firesteel App.");
    }

    void App::shutdown() {
        window.close();
    }

    bool drawn_ui = true;
    void App::update_loop_call() {
#ifdef XE_NTVX3
        nvtx3::scoped_range loop{ "update_loop_call" };
#endif // XE_NTVX3
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
#ifdef XE_NTVX3
        nvtx3::scoped_range r{ "update_app_function" };
#endif // XE_NTVX3
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