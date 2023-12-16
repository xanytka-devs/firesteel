#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Direction {
	NONE = 0,
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Camera {
public:
	Camera(glm::vec3 t_pos);
	void update_direction(double t_dx, double t_dy);
	void update_position(Direction t_dir, double t_dt);
	void update_zoom(double t_dy);

	glm::mat4 get_view_matrix();

	glm::vec3 position;

	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 world_up;

	float yaw;
	float pitch;
	float move_speed; //Temp.
	float zoom;
private:
	void update_vectors();
};

#endif // CAMERA_H