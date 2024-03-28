#ifndef APP_H
#define APP_H

#include "rendering/window.hpp"
#include "enviroment.hpp"

namespace XEngine {
	/// <summary>
	/// Base application class.
	/// </summary>
	class App {
	public:
		static App* instance() {
			return m_instance;
		}
		/// <summary>
		/// Occures at app startup (instantiation).
		/// </summary>
		App();
		/// <summary>
		/// Occures on app shutdown (destruction).
		/// </summary>
		virtual ~App();
		/// <summary>
		/// Quits app.
		/// </summary>
		void shutdown();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		/// <summary>
		/// Creates new instance of window for application.
		/// </summary>
		/// <param name="t_win_width">Width of the window.</param>
		/// <param name="t_win_height">Height of the window.</param>
		/// <param name="t_title">Name of the window.</param>
		/// <returns>Exit code. Only 0 is success.</returns>
		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);

		/// <summary>
		/// Runs before LITERALLY everything.
		/// </summary>
		virtual void preinitialize() { }
		/// <summary>
		/// Runs after renderer initialization.
		/// </summary>
		virtual void initialize() { }
		/// <summary>
		/// Runs every frame.
		/// </summary>
		virtual void update() { }
		/// <summary>
		/// Runs on app shutdown.
		/// </summary>
		virtual void on_shutdown() { }
		/// <summary>
		/// Dedicated update function.
		/// </summary>
		void update_loop_call();

		Window window;
		int fps = 0;
		/// <summary>
		/// Stop app updating for 1 frame.
		/// </summary>
		bool update_app = true;
	private:
		static App* m_instance;
	};

}

#endif // APP_H