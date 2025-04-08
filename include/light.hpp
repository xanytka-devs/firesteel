#ifndef FS_LIGHT_H
#define FS_LIGHT_H

#include <glm/glm.hpp>
#include "engine/include/shader.hpp"

namespace Firesteel {
	struct PointLight {
		glm::vec3 position;
		// Lighting params.
		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(1);
		glm::vec3 specular = glm::vec3(0.5f);
		// Helper func.
		void setParams(const Shader* tShader, const unsigned int tID) const {
			tShader->setVec3("pointLights[" + std::to_string(tID) + "].position", position);
			tShader->setVec3("pointLights[" + std::to_string(tID) + "].ambient", ambient);
			tShader->setVec3("pointLights[" + std::to_string(tID) + "].diffuse", diffuse);
			tShader->setVec3("pointLights[" + std::to_string(tID) + "].specular", specular);
		}
	};
	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		// Lighting params.
		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(1);
		glm::vec3 specular = glm::vec3(0.5f);
		// Attenuation.
		float cutOff = glm::cos(glm::radians(12.5f));
		float outerCutOff = glm::cos(glm::radians(17.5f));
		// Helper func.
		void setParams(const Shader* tShader, const unsigned int tID) const {
			tShader->setVec3( "spotLights[" + std::to_string(tID) + "].position", position);
			tShader->setVec3( "spotLights[" + std::to_string(tID) + "].direction", direction);
			tShader->setVec3( "spotLights[" + std::to_string(tID) + "].ambient", ambient);
			tShader->setVec3( "spotLights[" + std::to_string(tID) + "].diffuse", diffuse);
			tShader->setVec3( "spotLights[" + std::to_string(tID) + "].specular", specular);
			tShader->setFloat("spotLights[" + std::to_string(tID) + "].cutOff", cutOff);
			tShader->setFloat("spotLights[" + std::to_string(tID) + "].outerCutOff", outerCutOff);
		}
	};
}

#endif // !FS_LIGHT_H