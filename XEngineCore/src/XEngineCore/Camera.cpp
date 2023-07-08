#include "XEngineCore/Camera.hpp"
#include <glm/trigonometric.hpp>

namespace XEngine {

	Camera::Camera(const glm::vec3& pos,
		const glm::vec3& rot,
		const ProjectionMode mode)
		: position(pos), rotation(rot),
		projMode(mode) {
		updateViewMatrix();
		updateProjectionMatrix();
	}

	void Camera::setPosition(const glm::vec3& pos) {
		position = pos;
		updateViewMatrix();
	}
	void Camera::setRotation(const glm::vec3& rot) {
		rotation = rot;
		updateViewMatrix();
	}
	void Camera::setPositionRotation(const glm::vec3& pos, const glm::vec3& rot) {
		position = pos;
		rotation = rot;
		updateViewMatrix();
	}
	void Camera::setProjectionMode(const ProjectionMode mode) {
		projMode = mode;
		updateProjectionMatrix();
	}

	void Camera::updateViewMatrix() {
		//Rotate matrix.
		float rIRX = glm::radians(-rotation.x);
		glm::mat4 rotationXMatrix(1, 0, 0, 0, 0, cos(rIRX), -sin(rIRX), 0,
			0, sin(rIRX), cos(rIRX), 0, 0, 0, 0, 1);
		float rIRY = glm::radians(-rotation.y);
		glm::mat4 rotationYMatrix(cos(rIRY), 0, sin(rIRY), 0, 0, 1, 0, 0,
			-sin(rIRY), 0, cos(rIRY), 0, 0, 0, 0, 1);
		float rIRZ = glm::radians(-rotation.z);
		glm::mat4 rotationZMatrix(cos(rIRZ), sin(rIRZ), 0, 0,
			-sin(rIRZ), cos(rIRZ), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		//Translate matrix.
		glm::mat4 translateMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
			-position[0], -position[1], -position[2], 1);
		//Apply to view matrix.
		viewMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix * translateMatrix;
	}

	void Camera::updateProjectionMatrix() {

		if (projMode == ProjectionMode::Perspective) {
			float r = 0.1f;
			float t = 0.1f;
			float f = 10;
			float n = 0.1f;
			projectionMatrix = glm::mat4(n/r, 0, 0, 0, 0, n/t, 0, 0,
				0, 0, (-f-n)/(f-n), -1, 0, 0, -2 * f * n / (f-n), 0);
		} else {
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			projectionMatrix = glm::mat4(1/r, 0, 0, 0, 0, 1/t, 0, 0,
				0, 0, -2/(f-n), 0, 0, 0, (-f-n)/(f-n), 1);
		}

	}

}