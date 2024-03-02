#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <xengine/rendering/transform.hpp>
#include <xengine/rendering/light.hpp>

using namespace XEngine;

class LightSource : public Component {
public:
	LightSource() : Component() { }

	LightSource(Transform& t_transform, glm::vec4 t_color = glm::vec4(1.0f),
		glm::vec4 t_ambient = glm::vec4(0.5f),
		glm::vec4 t_diffuse = glm::vec4(1.0f),
		glm::vec4 t_specular = glm::vec4(1.0f),
		float t_k0 = 1.0f, float t_k1 = 0.07f, float t_k2 = 0.032f)
		: color(t_color), Component(t_transform), id(global_id), shader(nullptr) {
		light = XEngine::PointLight({ m_transform->position, t_k0, t_k1, t_k2, t_ambient, t_diffuse, t_specular });
	}

	void initialize() {
		id = global_id;
		global_id++;
		Component::initialize();
	}

	void update() {
		light.render(*shader, id);
	}

	static void reset_global_id() {
		global_id = 0;
	}

	int id;
	static int global_id;
	Shader* shader;
	glm::vec4 color;
	XEngine::PointLight light;
};

#endif // LIGHT_SOURCE_H