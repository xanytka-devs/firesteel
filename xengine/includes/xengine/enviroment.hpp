#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <glm/glm.hpp>

namespace XEngine {
	class Enviroment {
	public:
		static glm::vec3 gravity;
		static float delta_time;
	};
}

#endif // ENVIROMENT_H