#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <string>

#include "xengine/enviroment.hpp"
#include "xengine/rendering/transform.hpp"

namespace XEngine {
	enum ForceType {
		FT_None = 0,
		FT_Acceleration,
		FT_Force,
		FT_Impulse
	};

	class Rigidbody : public Component {
	public:
		Rigidbody() : Component(), acceleration(glm::vec3(0)), velocity(glm::vec3(0)), mass(0.f), use_gravity(true) { }

		Rigidbody(Transform& t_transform, float t_mass = 1.0f, glm::vec3 t_velocity = glm::vec3(0.f),
			glm::vec3 t_acceleration = glm::vec3(0.f), bool t_use_gravity = false)
			: mass(t_mass), velocity(t_velocity), acceleration(t_acceleration),
			use_gravity(t_use_gravity), Component(t_transform) { }

		virtual void update() override {
			//Calculate velocity.
			float dt = Enviroment::delta_time;
			m_transform->position += velocity * dt + 0.5f * acceleration * (dt * dt);
			velocity += acceleration * dt;
			//Calculate gravity.
			if(use_gravity) velocity += Enviroment::gravity * dt;
		}

		/// <summary>
		/// Adds force to object.
		/// </summary>
		/// <param name="t_type">Type of force.</param>
		/// <param name="t_direction">Direction.</param>
		/// <param name="t_magnitude">Magnitude.</param>
		void add_force(ForceType t_type, glm::vec3 t_direction, float t_magnitude) {
			add_force(t_type, t_direction * t_magnitude);
		}

		/// <summary>
		/// Adds force to object.
		/// </summary>
		/// <param name="t_type">Type of force.</param>
		/// <param name="t_value">Amount.</param>
		void add_force(ForceType t_type, glm::vec3 t_value) {
			switch (t_type) {
			case XEngine::FT_Acceleration:
				acceleration += t_value;
				break;
			case XEngine::FT_Force:
				acceleration += t_value / mass;
				break;
			case XEngine::FT_Impulse:
				velocity += t_value / mass * Enviroment::delta_time;
				break;
			default:
				break;
			}
		}

		/// <summary>
		/// Transfers energy.
		/// </summary>
		/// <param name="t_direction">Direction of energy.</param>
		/// <param name="t_val">Amount of energy.</param>
		void transfer_energy(glm::vec3 t_direction, float t_val) {
			//If no energy - return.
			if(t_val == 0) return;
			//Ek= 0,5 * m * v^2
			glm::vec3 delta_v = t_direction * sqrt(2 * abs(t_val) / mass);
			velocity += t_val > 0 ? delta_v : -delta_v;
		}

		float mass;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		bool use_gravity;
	};
}

#endif // RIGIDBODY_H