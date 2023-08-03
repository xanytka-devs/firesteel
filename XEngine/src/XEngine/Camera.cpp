#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "XEngine/Camera.hpp"

namespace XEngine {

	float Camera::farClipPlane = 100.f;
	float Camera::nearClipPlane = 0.1f;
	float Camera::viewportWidth = 800.f;
	float Camera::viewportHeight = 600.f;
	float Camera::fieldOfView = 60.f;

	Camera::Camera(const glm::vec3& pos,
		const glm::vec3& rot,
		const ProjectionMode mode)
		: position(pos), rotation(rot),
		projMode(mode) {
		updateViewMatrix();
		updateProjectionMatrix();
	}

	const glm::mat4& Camera::getViewMatrix() {
		if (doUpdateViewMatrix) {
			updateViewMatrix();
			doUpdateViewMatrix = false;
		}
		return viewMatrix;
	}

	/*void Camera::Transform::onPositionChanged() {
		Camera::doUpdateViewMatrix = true;
	}
	void Camera::Transform::onRotationChanged() {
		Camera::doUpdateViewMatrix = true;
	}
	void Camera::Transform::onScaleChanged() {
		return;
	}*/

	void Camera::setPosition(const glm::vec3& pos) {
		position = pos;
		doUpdateViewMatrix = true;
	}
	void Camera::setRotation(const glm::vec3& rot) {
		rotation = rot;
		doUpdateViewMatrix = true;
	}
	void Camera::moveForward(const float delta) {
		position += forward * delta;
		doUpdateViewMatrix = true;
	}
	void Camera::moveRight(const float delta) {
		position += right * delta;
		doUpdateViewMatrix = true;
	}
	void Camera::moveUp(const float delta) {
		position += up * delta;
		doUpdateViewMatrix = true;
	}

	void Camera::moveRot(const glm::vec3& movDelta, const glm::vec3& rotDelta) {
		position += forward * movDelta.x;
		position += right * movDelta.y;
		position += up * movDelta.z;
		rotation += rotDelta;
		doUpdateViewMatrix = true;
	}

	void Camera::setPositionRotation(const glm::vec3& pos, const glm::vec3& rot) {
		position = pos;
		rotation = rot;
		doUpdateViewMatrix = true;
	}
	void Camera::setProjectionMode(const ProjectionMode mode) {
		projMode = mode;
		updateProjectionMatrix();
	}

	void Camera::updateViewMatrix() {
		//Calculate radians of rotations.
		const float rollRads = glm::radians(rotation.x);
		const float pitchRads = glm::radians(rotation.y);
		const float yawRads = glm::radians(rotation.z);
		//Rotate matrix.
		const glm::mat3 rotate_matrix_x(1, 0, 0,
			0, cos(rollRads), sin(rollRads), 0, -sin(rollRads), cos(rollRads));
		const glm::mat3 rotate_matrix_y(cos(pitchRads), 0, -sin(pitchRads),
			0, 1, 0, sin(pitchRads), 0, cos(pitchRads));
		const glm::mat3 rotate_matrix_z(cos(yawRads), sin(yawRads), 0,
			-sin(yawRads), cos(yawRads), 0, 0, 0, 1);
		//Calculate euler.
		const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
		forward = glm::normalize(euler_rotate_matrix * worldForward);
		right = glm::normalize(euler_rotate_matrix * worldRight);
		up = glm::cross(right, forward);
		//Apply to view matrix.
		viewMatrix = glm::lookAt(position, position + forward, up);
	}

	void Camera::updateProjectionMatrix() {
		if (projMode == ProjectionMode::Perspective)
			projectionMatrix = glm::perspective(glm::radians(fieldOfView), viewportWidth / viewportHeight,
				nearClipPlane, farClipPlane);
		else {
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			projectionMatrix = glm::mat4(1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1);
		}
	}

	void Camera::setViewportSize(const float width, const float height) {
		viewportWidth = width;
		viewportHeight = height;
		updateProjectionMatrix();
	}

	void Camera::setViewportWidth(const float width) {
		viewportWidth = width;
		updateProjectionMatrix();
	}
	void Camera::setViewportHeight(const float height) {
		viewportHeight = height;
		updateProjectionMatrix();
	}
	void Camera::setNearClipPlane(const float value) {
		nearClipPlane = value;
		updateProjectionMatrix();
	}
	void Camera::setFarClipPlane(const float value) {
		farClipPlane = value;
		updateProjectionMatrix();
	}
	void Camera::setFieldOfView(const float value) {
		fieldOfView = value;
		if(fieldOfView <= 0) fieldOfView = 179.0f;
		if(fieldOfView >= 180) fieldOfView = 1.0f;
		updateProjectionMatrix();
	}

}