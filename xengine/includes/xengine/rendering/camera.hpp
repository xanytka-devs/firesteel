#ifndef CAMERA_H
#define CAMERA_H

#include "xengine/common.hpp"

namespace XEngine {

	class Camera {
	public:
		Camera();
		/// <summary>
		/// Sets up camera.
		/// </summary>
		/// <param name="t_pos">Position of new camera.</param>
		Camera(glm::vec3 t_pos);
		/// <summary>
		/// Sets up camera.
		/// </summary>
		/// <param name="t_pos">Position of new camera.</param>
		/// <param name="t_yaw">Yaw.</param>
		/// <param name="t_pitch">Pitch.</param>
		Camera(glm::vec3 t_pos, float t_yaw, float t_pitch);

		/// <summary>
		/// Updates camera vectors.
		/// </summary>
		void update_vectors();
		/// <summary>
		/// Calculate view matrix.
		/// </summary>
		/// <returns>View matrix.</returns>
		glm::mat4 get_view_matrix() const;
		/// <summary>
		/// Calculate projection matrix.
		/// </summary>
		/// <param name="t_clip_size">Aspect.</param>
		/// <returns>Projection matrix.</returns>
		glm::mat4 get_projection_matrix(float t_clip_size = 1);

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 world_up;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		float fov;

		float near_plane;
		float far_plane;
		float aspect;
		bool is_perspective;
	};
}

#endif // CAMERA_H