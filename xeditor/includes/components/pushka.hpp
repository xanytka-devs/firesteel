#include <XEngine/Rendering/Camera.hpp>
#include <XEngine/Rendering/Transform.hpp>
#include <XEngine/Input.hpp>

class Pushka : public XEngine::Transform {
public:
	XEngine::Camera* cam;

	Pushka(XEngine::Camera* t_cam) : Transform(glm::vec3(0.0f), glm::vec4(glm::vec3(0.f), 1.0f), glm::vec3(0.75f)), cam(t_cam) {}

	void initialize() {
		Transform::load_model("..\\..\\res\\desert_eagle\\scene.gltf");
	}

	void render(XEngine::Shader shader) {

		// set position
		// multiply offset by unit vector in 2 directions
		Transform::position = position = (*cam).position + glm::vec3((*cam).forward * 2.f) + glm::vec3((*cam).up * 0.f);
		Transform::rotation = rotation = glm::vec4(cam->rotation.x, cam->rotation.z, cam->rotation.y, 1.f);

		/*float theta;
		glm::mat4 model = glm::mat4(1.0f);

		// rotate around camera right using dot product
		theta = acos(glm::dot((*cam).up, (*cam).forward) /
			glm::length((*cam).up) / glm::length((*cam).forward));
		model = glm::rotate(model, atanf(1) * 2 - theta, (*cam).right); // offset by pi/2 radians bc angle btwn camFront and gunFront

		// rotate around cameraUp using dot product
		glm::vec2 front2d = glm::vec2((*cam).forward.x, (*cam).forward.z);
		theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, (*cam).forward.z < 0 ? theta : -theta, (*cam).up);*/

		Transform::render(shader);
	}
};