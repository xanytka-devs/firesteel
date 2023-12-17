#ifndef RENDERER_H
#define RENDERER_H

namespace XEngine {
	enum RenderMode {
		DEFAULT = 0,
		WIREFRAME
	};

	class Renderer {
	public:
		static void initialize();
		static bool initialize_libs();
		static void terminate();
		static void switch_mode(RenderMode t_mode);
		static double get_time();
	};
}

#endif // RENDERER_H