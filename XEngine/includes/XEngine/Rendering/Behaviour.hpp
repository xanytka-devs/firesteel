#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "XEngine/Rendering/Mesh.hpp"

namespace XEngine {
	class Behaviour {
	public:
		Behaviour();
		void initialize();
		void render(Shader t_shader);
		void remove();

		std::vector<Mesh> meshes;
	};
}

#endif // MODEL_H