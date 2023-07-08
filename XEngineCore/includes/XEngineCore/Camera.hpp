#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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
		glm::mat4 getViewMatrix() const { return viewMatrix; }
		glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
	private:
		glm::vec3 position;
		glm::vec3 rotation;
		ProjectionMode projMode;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		void updateViewMatrix();
		void updateProjectionMatrix();
	};

}