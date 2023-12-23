#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "XEngine/Components/Transform.hpp"
namespace XEngine {
	class LightSource : public Transform {
	public:
		LightSource(glm::vec3 t_color = glm::vec3(1.0f),
			glm::vec3 t_ambient = glm::vec3(1.0f),
			glm::vec3 t_diffuse = glm::vec3(1.0f),
			glm::vec3 t_specular = glm::vec3(1.0f),
			glm::vec3 t_pos = glm::vec3(1.0f),
			glm::vec4 t_rotation = glm::vec4(1.0f),
			glm::vec3 t_size = glm::vec3(1.0f))
			: color(t_color), ambient(t_ambient), diffuse(t_diffuse), specular(t_specular),
			Transform(t_pos, t_rotation, t_size, Material::white_plastic) {
			shoud_rotate = false;
		}

		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		void render(Shader t_shader) {
			t_shader.set_3_floats("light_color", color);
			Transform::render(t_shader);
		}
	};
}

#endif // LIGHT_SOURCE_H