#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace XEngine {

	class Camera {
	public:
		enum class ProjectionMode {
			Perspective,
			Ortographic
		};

		Camera(const glm::vec3& pos = { 0, 0, 0 },
			const glm::vec3& rot = { 0, 0, 0 },
			const ProjectionMode mode = ProjectionMode::Perspective);

		void setPosition(const glm::vec3& pos);
		void setRotation(const glm::vec3& rot);
		void setPositionRotation(const glm::vec3& pos, const glm::vec3& rot);
		void setProjectionMode(const ProjectionMode mode);
		void setViewportWidth(const float width);
		void setViewportHeight(const float height);
		void setViewportSize(const float width, const float height);
		void setNearClipPlane(const float value);
		void setFarClipPlane(const float value);
		void setFieldOfView(const float value);
		void moveForward(const float delta);
		void moveRight(const float delta);
		void moveUp(const float delta);
		void moveRot(const glm::vec3& movDelta, const glm::vec3& rotDelta);

		const glm::mat4& getViewMatrix();
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getRotation() const { return rotation; }

		static float farClipPlane;
		static float nearClipPlane;
		static float viewportWidth;
		static float viewportHeight;
		static float fieldOfView;
	private:
		void updateViewMatrix();
		void updateProjectionMatrix();

		glm::vec3 position;
		glm::vec3 rotation; // X=Roll,Y=Pitch,Z=Yaw
		ProjectionMode projMode;

		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;

		static constexpr glm::vec3 worldUp{ 0.f, 0.f, 1.f };
		static constexpr glm::vec3 worldRight{ 0.f, -1.f, 0.f };
		static constexpr glm::vec3 worldForward{ 1.f, 0.f, 0.f };

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		bool doUpdateViewMatrix = false;
	};

}