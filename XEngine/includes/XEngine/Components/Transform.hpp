#pragma once

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

namespace XEngine {

	class Transform {
	public:
		Transform(const glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f),
			const glm::vec3 rot = glm::vec3(0.f, 0.f, 0.f),
			const glm::vec3 scale = glm::vec3(0.f, 0.f, 0.f));
		~Transform();

		glm::vec3 position;
		glm::vec3 localPosition;
		glm::vec3 rotation; // X=Roll,Y=Yaw,Z=Pitch
		glm::vec3 localRotation;
		glm::vec3 scale;
		glm::vec3 localScale;

		glm::vec3 forward;
		glm::vec3 back;
		glm::vec3 right;
		glm::vec3 left;
		glm::vec3 up;
		glm::vec3 down;

		static constexpr glm::vec3 worldUp{ 0.f, 1.f, 0.f };
		static constexpr glm::vec3 worldRight{ 0.f, 0.f, -1.f };
		static constexpr glm::vec3 worldForward{ 1.f, 0.f, 0.f };

		const void onPositionChanged() { modelMatrix = CalculateMatrix(); }
		const void onRotationChanged() { modelMatrix = CalculateMatrix(); }
		const void onScaleChanged() { modelMatrix = CalculateMatrix(); }

		void setPosition(const glm::vec3& pos);
		void setRotation(const glm::vec3& rot);
		void setScale(const glm::vec3& scale);
		void moveForward(const float delta);
		void moveRight(const float delta);
		void moveUp(const float delta);
		void moveRot(const glm::vec3& movDelta, const glm::vec3& rotDelta);
	private:
		glm::mat4 CalculateMatrix();
		glm::mat4 modelMatrix;
	};

}