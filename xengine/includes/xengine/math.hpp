//Includes GLM without need to add it to CMake project.
#include "../../external/glm/glm/glm.hpp"
#include "../../external/glm/glm/gtc/type_ptr.hpp"

#ifndef MATH_H
#define MATH_H

namespace XEngine {
	class Rand {
		/// <summary>
		/// Gets random number.
		/// </summary>
		/// <returns>Random integer.</returns>
		static int get_int() {
			return rand();
		}
		/// <summary>
		/// Gets random number.
		/// </summary>
		/// <param name="t_seed">Randomization base.</param>
		/// <returns>Random integer (depending on seed).</returns>
		static int get_int(int t_seed) {
			srand(t_seed);
			return rand();
		}
		/// <summary>
		/// Gets random number between two values.
		/// </summary>
		/// <param name="t_min">Minimum value.</param>
		/// <param name="t_max">Maximum value.</param>
		/// <returns>Random integer.</returns>
		static int get_int(int t_min, int t_max) {
			return rand() % (t_max - t_min + 1) + t_min;
		}
		/// <summary>
		/// Gets random number between two values.
		/// </summary>
		/// <param name="t_seed">Randomization base.</param>
		/// <param name="t_min">Minimum value.</param>
		/// <param name="t_max">Maximum value.</param>
		/// <returns>Random integer (depending on seed).</returns>
		static int get_int(int t_seed, int t_min, int t_max) {
			srand(t_seed);
			return rand() % (t_max - t_min + 1) + t_min;
		}
	};
}

#endif // MATH_H