#ifndef CAMERA_H
#define CAMERA_H

#include "firesteel/common.hpp"

namespace firesteel {

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
		/// <param name="t_roll">Roll (X).</param>
		/// <param name="t_yaw">Yaw (Y).</param>
		/// <param name="t_pitch">Pitch (Z).</param>
		Camera(glm::vec3 t_pos, float t_roll, float t_yaw, float t_pitch);
		/// <summary>
		/// Sets up camera.
		/// </summary>
		/// <param name="t_pos">Position of new camera.</param>
		/// <param name="t_yaw">Rotation.</param>
		Camera(glm::vec3 t_pos, glm::vec3 t_rot);

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
		glm::vec3 rotation;
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