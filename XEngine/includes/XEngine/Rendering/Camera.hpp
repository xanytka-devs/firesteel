#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace XEngine {

	class Camera {
	public:
		Camera();
		Camera(glm::vec3 t_pos);
		Camera(glm::vec3 t_pos, float t_yaw, float t_pitch);

		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix(float t_clip_size = 1);
		void update_vectors();

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