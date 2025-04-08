#ifndef FS_ATMOSPHERE_H
#define FS_ATMOSPHERE_H

#include "engine/include/common.hpp"
#include "engine/include/shader.hpp"
#include "engine/include/cubemap.hpp"

namespace Firesteel {
	struct Atmosphere {
		struct DirectionalLight {
			glm::vec3 direction;
			// Lighting params.
			glm::vec3 ambient = glm::vec3(0.2f);
			glm::vec3 diffuse = glm::vec3(1);
			glm::vec3 specular = glm::vec3(0.5f);
			// Helper func.
			void setParams(const Shader* tShader) const {
				tShader->setVec3("dirLight.direction", direction);
				tShader->setVec3("dirLight.ambient", ambient);
				tShader->setVec3("dirLight.diffuse", diffuse);
				tShader->setVec3("dirLight.specular", specular);
			}
		} directionalLight;
#define FOG_LINEAR 0
#define FOG_EXP 1
#define FOG_EXP2 2
		struct Fog {
			float density = 0.04f;
			glm::vec3 color = glm::vec3(0.2f);
			int equation = FOG_EXP;
			float start = 20.f;
			float end = 25.f;
			void setParams(const Shader* tShader) const {
				tShader->setVec3("fogParams.vFogColor", color);
				tShader->setFloat("fogParams.fStart", start);
				tShader->setFloat("fogParams.fEnd", end);
				tShader->setFloat("fogParams.fDensity", density);
				tShader->setInt("fogParams.iEquation", equation);
			}
		} fog;
		void setParams(const Shader* tShader) const {
			directionalLight.setParams(tShader);
			fog.setParams(tShader);
		}
	};

}


#endif // !FS_ATMOSPHERE_H
