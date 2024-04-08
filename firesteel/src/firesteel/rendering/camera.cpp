#include "firesteel/rendering/camera.hpp"

namespace firesteel {

	Camera::Camera() 
		: position(glm::vec3(0.f)), world_up(glm::vec3(0.f, 1.f, 0.f)),
		rotation(glm::vec4(0.f, 0.f, -90.f, 1.f)), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)),
		is_perspective(true), far_plane(100.f), near_plane(0.1f),
		up(glm::vec3(0.f)), right(glm::vec3(0.f)), aspect(0.5f) { }

	Camera::Camera(glm::vec3 t_pos)
		: position(t_pos), world_up(glm::vec3(0.f, 1.f, 0.f)),
		rotation(glm::vec4(0.f, 0.f, -90.f, 1.f)), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)),
		is_perspective(true), far_plane(100.f), near_plane(0.1f) {
		update_vectors();
	}

	Camera::Camera(glm::vec3 t_pos, float t_yaw, float t_pitch)
		: position(t_pos), world_up(glm::vec3(0.f, 1.f, 0.f)),
		rotation(glm::vec4(0.f, t_pitch, t_yaw, 1.f)), fov(45.f), forward(glm::vec3(0.f, 0.f, -1.f)),
		is_perspective(true), far_plane(100.f), near_plane(0.1f) {
		update_vectors();
	}

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

	glm::mat4 Camera::get_view_matrix() const {
		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 Camera::get_projection_matrix(float t_clip_size) {
		if(is_perspective) return glm::perspective(glm::radians(fov), t_clip_size * aspect, near_plane, far_plane);
		else return glm::ortho(-t_clip_size, t_clip_size,
			-t_clip_size * aspect, t_clip_size * aspect, near_plane, far_plane);
	}

}