#include "xengine/physics/rigidbody.hpp"

namespace XEngine {

	Rigidbody::Rigidbody(Transform t_transform, float t_mass, glm::vec3 t_velocity, glm::vec3 t_acceleration)
	: mass(t_mass), velocity(t_velocity), acceleration(t_acceleration), Component(t_transform) {
	}
	void Rigidbody::update() {

	}

}