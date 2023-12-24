#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "XEngine/Components/Cube.hpp"
#include "XEngine/Rendering/Light.hpp"

namespace XEngine {
	class LightSource : public Cube {
	public:
		LightSource(glm::vec3 t_color = glm::vec3(1.0f),
			glm::vec3 t_ambient = glm::vec3(1.0f),
			glm::vec3 t_diffuse = glm::vec3(1.0f),
			glm::vec3 t_specular = glm::vec3(1.0f),
			float t_k0 = 1.0f, float t_k1 = 0.07f, float t_k2 = 0.032f,
			glm::vec3 t_pos = glm::vec3(1.0f),
			glm::vec4 t_rotation = glm::vec4(1.0f),
			glm::vec3 t_size = glm::vec3(1.0f))
			: color(t_color), light({ t_pos, t_k0, t_k1, t_k2, t_ambient, t_diffuse,t_specular }),
			Cube(t_pos, t_rotation, t_size) {
			shoud_rotate = false;
		}
		void render(Shader t_shader) {
			t_shader.set_3_floats("light_color", color);
			Cube::render(t_shader);
		}

		glm::vec3 color;
		PointLight light;
	};
}

#endif // LIGHT_SOURCE_H