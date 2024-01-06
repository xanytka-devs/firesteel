#include "XEngine/Rendering/Camera.hpp"

namespace XEngine {

	Camera::Camera() { }

	/// <summary>
	/// Sets up camera.
	/// </summary>
	/// <param name="t_pos">Position of new camera.</param>
	Camera::Camera(glm::vec3 t_pos)
		: position(t_pos), world_up(glm::vec3(0.f, 1.f, 0.f)),
		rotation(glm::vec4(0.f, 0.f, -90.f, 1.f)), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)),
		is_perspective(true), far_plane(100.f), near_plane(0.1f) {
		update_vectors();
	}

	/// <summary>
	/// Sets up camera.
	/// </summary>
	/// <param name="t_pos">Position of new camera.</param>
	/// <param name="t_yaw">Yaw.</param>
	/// <param name="t_pitch">Pitch.</param>
	Camera::Camera(glm::vec3 t_pos, float t_yaw, float t_pitch)
		: position(t_pos), world_up(glm::vec3(0.f, 1.f, 0.f)),
		rotation(glm::vec4(0.f, t_pitch, t_yaw, 1.f)), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)),
		is_perspective(true), far_plane(100.f), near_plane(0.1f) {
		update_vectors();
	}

	/// <summary>
	/// Updates camera vectors.
	/// </summary>
	void Camera::update_vectors() {
		//Get directions.
		glm::vec3 dir{};
		float roll = glm::radians(rotation.x);
		float pitch = glm::radians(rotation.y);
		float yaw = glm::radians(rotation.z);
		dir.x = cos(yaw) * cos(pitch);
		dir.y = sin(pitch);
		dir.z = sin(yaw) * cos(pitch);
		//Calculate directions.
		forward = glm::normalize(dir);
		right = glm::normalize(glm::cross(forward, world_up));
		up = glm::normalize(glm::cross(right, forward));
	}

	/// <summary>
	/// Calculate view matrix.
	/// </summary>
	/// <returns></returns>
	glm::mat4 Camera::get_view_matrix() const {
		return glm::lookAt(position, position + forward, up);
	}

	/// <summary>
	/// Calculate projection matrix.
	/// </summary>
	/// <param name="t_clip_size"></param>
	/// <returns></returns>
	glm::mat4 Camera::get_projection_matrix(float t_clip_size) {
		if(is_perspective) return glm::perspective(glm::radians(fov), t_clip_size * aspect, near_plane, far_plane);
		else return glm::ortho(-t_clip_size, t_clip_size,
			-t_clip_size * aspect, t_clip_size * aspect, near_plane, far_plane);
	}

}