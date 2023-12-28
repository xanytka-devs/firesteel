#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace XEngine {

	class Camera {
	public:
		Camera();
		Camera(glm::vec3 t_pos);
		void update_direction(double t_dx, double t_dy);

		glm::mat4 get_view_matrix();
		glm::mat4 get_projection_matrix(float t_clip_size = 1);

		glm::vec3 position;
		glm::vec3 world_up;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		float yaw;
		float pitch;
		float fov;

		float near_plane;
		float far_plane;
		float aspect;
		bool is_perspective;
	private:
		void update_vectors();
	};
}

#endif // CAMERA_H