//Includes GLM without need to add it to CMake project.
#include "../../external/glm/glm/glm.hpp"

#ifndef MATH_H
#define MATH_H

#include "xengine/rendering/renderer.hpp"

namespace XEngine {
	class Rand {
		static int get_int() {
			srand((int)Renderer::get_time());
			return rand();
		}
		static int get_int(int t_seed) {
			srand(t_seed);
			return rand();
		}
		static int get_int(int t_min, int t_max) {
			srand((int)Renderer::get_time());
			return rand() % (t_max - t_min + 1) + t_min;
		}
	};
}

#endif // MATH_H