#include "XEngine/Rendering/Camera.hpp"

namespace XEngine {

	Camera::Camera() { }

	/// <summary>
	/// Sets up camera.
	/// </summary>
	/// <param name="t_pos">Position of new camera.</param>
	Camera::Camera(glm::vec3 t_pos)
		: position(t_pos), world_up(glm::vec3(0.f, 1.f, 0.f)),
		yaw(-90.f), pitch(0.f), move_speed(2.5f), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)) {
		update_vectors();
	}

	/// <summary>
	/// Updates camera vectors.
	/// </summary>
	void Camera::update_vectors() {
		//Get directions.
		glm::vec3 dir{};
		dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dir.y = sin(glm::radians(pitch));
		dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		//Calculate directions.
		forward = glm::normalize(dir);
		right = glm::normalize(glm::cross(forward, world_up));
		up = glm::normalize(glm::cross(right, forward));
	}

	/// <summary>
	/// Temp. | For camera rotation.
	/// </summary>
	/// <param name="t_dx">X movement.</param>
	/// <param name="t_dy">Y movement.</param>
	void Camera::update_direction(double t_dx, double t_dy) {
		//Update yaw and pitch.
		yaw += t_dx;
		pitch += t_dy;
		//Limit pitch.
		if (pitch > 89.f) pitch = 89.f;
		else if (pitch < -89.f) pitch = -89.f;
		//Update vectors.
		update_vectors();
	}

	/// <summary>
	/// Temp. | For camera movement.
	/// </summary>
	/// <param name="t_dir">Direction to move in.</param>
	/// <param name="t_dt">Delta time.</param>
	void Camera::update_position(Direction t_dir, double t_dt) {
		//Get velocity.
		float velocity = (float)t_dt * move_speed;
		//Change position.
		switch (t_dir) {
		case Direction::FORWARD:
			position += forward * velocity;
			break;
		case Direction::BACK:
			position -= forward * velocity;
			break;
		case Direction::LEFT:
			position -= right * velocity;
			break;
		case Direction::RIGHT:
			position += right * velocity;
			break;
		case Direction::UP:
			position += up * velocity;
			break;
		case Direction::DOWN:
			position -= up * velocity;
			break;
		default:
			break;
		}
	}

	/// <summary>
	/// Calculate view matrix.
	/// </summary>
	/// <returns></returns>
	glm::mat4 Camera::get_view_matrix() {
		return glm::lookAt(position, position + forward, up);
	}

}