#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>

#include "xengine/component.hpp"
#include "xengine/enviroment.hpp"

namespace XEngine {
	class Rigidbody : public Component {
	public:
		Rigidbody(Transform t_transform, float t_mass = 1.0f, glm::vec3 t_velocity = glm::vec3(0.f),
			glm::vec3 t_acceleration = glm::vec3(0.f))
			: mass(t_mass), velocity(t_velocity), acceleration(t_acceleration), Component(t_transform) {
		}
		void update() {
			float dt = Enviroment::delta_time;
			m_transform->position += velocity * dt + 0.5f * acceleration * (dt * dt);
			velocity += acceleration * dt;
		}

		float mass;
		glm::vec3 velocity;
		glm::vec3 acceleration;
	};
}

#endif // RIGIDBODY_H