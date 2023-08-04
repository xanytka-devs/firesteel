#pragma once

#include <iostream>

#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace XEngine::Rendering {

	enum ShaderType {
		SH_Vertex,
		SH_Geometry,
		SH_Fragment,
		SH_Compute,
	};

	class Material {
	public:
		Material(const char* name, const char* vertShader, const char* fragShader);
		~Material();

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&& mat) noexcept;
		Material(Material&& mat) noexcept;

		void setMatrix4(const char* name, const glm::mat4& value) const { program->setMatrix4(name, value); }
		void setMatrix3(const char* name, const glm::mat3& value) const { program->setMatrix3(name, value); }
		void setInt(const char* name, const int value) const { program->setInt(name, value); }
		void setFloat(const char* name, const float value) const { program->setFloat(name, value); }
		void setVector3(const char* name, const glm::vec3 value) const { program->setVector3(name, value); }

		void addShader(const ShaderType param, const char* value);
		void addTexture(const Texture2D* value);

		ShaderProgram* getShaderProgram() { return program.get(); }
		void bind() { program->bind(); }
		void unbind() { program->unbind(); }
	private:
		unsigned int curID = 0;
		char* name;
		size_t assignedShaders = 0;
		size_t assignedTextures = 0;
		std::unique_ptr<ShaderProgram> program;
	};

}