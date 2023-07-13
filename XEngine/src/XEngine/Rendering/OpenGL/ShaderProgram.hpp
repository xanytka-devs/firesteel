#pragma once

#include <glm/mat4x4.hpp>

namespace XEngine::OpenGL {

	class ShaderProgram {

	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		~ShaderProgram();
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;

		void bind() const;
		static void unbind();
		bool isCompilied() const { return pIsCompiled; }
		void setMatrix4(const char* name, const glm::mat4& value) const;
		void setMatrix3(const char* name, const glm::mat3& value) const;
		void setInt(const char* name, const int value) const;
		void setFloat(const char* name, const float value) const;
		void setVector3(const char* name, const glm::vec3 value) const;

	private:
		bool pIsCompiled = false;
		unsigned int curID = 0;

	};

}