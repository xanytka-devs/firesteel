#include <glm/trigonometric.hpp>

#include "XEngine/Components/Transform.hpp"

namespace XEngine {

	Transform::Transform(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale)
	: position(pos), rotation(rot), scale(scale) {
		localPosition = pos;
		localRotation = rot;
		localScale = scale;
		forward = pos * worldForward;
		back = pos * -worldForward;
		right = pos * worldRight;
		left = pos * -worldRight;
		up = pos * worldUp;
		down = pos * -worldUp;
		modelMatrix = CalculateMatrix();
	}
	Transform::~Transform() {

	}

	void Transform::setPosition(const glm::vec3& pos) {
		position = pos;
		onPositionChanged();
	}
	void Transform::setRotation(const glm::vec3& rot) {
		rotation = rot;
		onRotationChanged();
	}
	void Transform::setScale(const glm::vec3& scale) {
		this->scale = scale;
		onScaleChanged();
	}

	void Transform::moveForward(const float delta) {
		position += forward * delta;
		onPositionChanged();
	}
	void Transform::moveRight(const float delta) {
		position += right * delta;
		onPositionChanged();
	}
	void Transform::moveUp(const float delta) {
		position += up * delta;
		onPositionChanged();
	}
	void Transform::moveRot(const glm::vec3& movDelta, const glm::vec3& rotDelta) {
		position += forward * movDelta.x;
		position += right * movDelta.y;
		position += up * movDelta.z;
		rotation += rotDelta;
		onPositionChanged();
		onRotationChanged();
	}
	glm::mat4 Transform::CalculateMatrix() {
		//Scale matrix.
		glm::mat4 scaleMatrix(scale[0], 0, 0, 0, 0, scale[1], 0, 0, 0, 0, scale[2], 0, 0, 0, 0, 1);
		//Rotate matrix.
		float rIRX = glm::radians(rotation[0]);
		glm::mat4 rotationXMatrix(1, 0, 0, 0, 0, cos(rIRX), -sin(rIRX), 0,
			0, sin(rIRX), cos(rIRX), 0, 0, 0, 0, 1);
		float rIRY = glm::radians(rotation[1]);
		glm::mat4 rotationYMatrix(cos(rIRY), 0, sin(rIRY), 0, 0, 1, 0, 0,
			-sin(rIRY), 0, cos(rIRY), 0, 0, 0, 0, 1);
		float rIRZ = glm::radians(rotation[2]);
		glm::mat4 rotationZMatrix(cos(rIRZ), sin(rIRZ), 0, 0,
			-sin(rIRZ), cos(rIRZ), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		//Translate matrix.
		glm::mat4 positionMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
			position[0], position[1], position[2], 1);
		//Model matrix.
		return scaleMatrix * (rotationXMatrix * rotationYMatrix * rotationZMatrix) * positionMatrix;
	}

}