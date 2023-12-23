#ifndef MODEL_H
#define MODEL_H

#include "XEngine/Rendering/Mesh.hpp"

namespace XEngine {
	class Behaviour {
	public:
		Behaviour();
		void initialize();
		void render(Shader t_shader);
		void remove();

		Mesh mesh;
	};
}

#endif // MODEL_H