#include <glad/glad.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include <XEngine/Rendering/OpenGL/ShaderProgram.hpp>
#include <XEngine/Log.hpp>

namespace XEngine::OpenGL {

	/// <summary>
	/// Tries compiling given shader.
	/// </summary>
	/// <param name="src">Code of shader.</param>
	/// <param name="shaderType">Shader type.</param>
	/// <param name="shaderID">Shader instance.</param>
	/// <returns>Success of compilation.</returns>
	bool createShader(const char* src, const GLenum shaderType, GLuint& shaderID) {
		//Instantiate and compile shader.
		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &src, nullptr);
		glCompileShader(shaderID);
		//Check compilation status.
		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			//Get what went wrong.
			char infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			//Tell about error in log.
			LOG_CRIT("Shader compilation error.\nMore info:\n{}", infoLog);
			return false;
		}
		return true;
	}

	/// <summary>
	/// Creates OpenGL program and compilies it.
	/// </summary>
	/// <param name="vertexShaderSrc">Code of vertex shader.</param>
	/// <param name="fragmentShaderSrc">Code of fragment shader.</param>
	ShaderProgram::ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) {
		//Try compiling vertex shader.
		GLuint vertexShaderID = 0;
		if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderID)) {
			LOG_CRIT("Vertex Shader compilation error");
			glDeleteShader(vertexShaderID);
			return;
		}
		//Try compiling fragment shader.
		GLuint fragmentShaderID = 0;
		if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			LOG_CRIT("Fragment Shader compilation error");
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			return;
		}
		//Link shaders.
		curID = glCreateProgram();
		glAttachShader(curID, vertexShaderID);
		glAttachShader(curID, fragmentShaderID);
		glLinkProgram(curID);
		//Check compilation status.
		GLint success;
		glGetProgramiv(curID, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			//Get what went wrong.
			char infoLog[1024];
			glGetShaderInfoLog(curID, 1024, nullptr, infoLog);
			//Tell about error in log.
			LOG_CRIT("Shader compilation error.\nMore info:\n{}", infoLog);
			//Delete program.
			glDeleteProgram(curID);
			curID = 0;
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			return;
		}
		pIsCompiled = true;
		//Detach shaders.
		glDetachShader(curID, vertexShaderID);
		glDetachShader(curID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}
	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(curID);
	}

	/// <summary>
	/// Applies current program to OpenGL.
	/// </summary>
	void ShaderProgram::bind() const {
		glUseProgram(curID);
	}

	/// <summary>
	/// Removes current program from OpenGL.
	/// </summary>
	void ShaderProgram::unbind() {
		glUseProgram(0);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		//Move data to this instance.
		glDeleteProgram(curID);
		curID = shaderProgram.curID;
		pIsCompiled = shaderProgram.pIsCompiled;
		//Clean other instance.
		shaderProgram.curID = 0;
		shaderProgram.pIsCompiled = false;
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
		//Move data to this instance.
		curID = shaderProgram.curID;
		pIsCompiled = shaderProgram.pIsCompiled;
		//Clean other instance.
		shaderProgram.curID = 0;
		shaderProgram.pIsCompiled = false;
	}

	void ShaderProgram::setMatrix3(const char* name, const glm::mat3& value) const {
		glUniformMatrix3fv(glGetUniformLocation(curID, name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void ShaderProgram::setMatrix4(const char* name, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(curID, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::setInt(const char* name, const int value) const {
		glUniform1i(glGetUniformLocation(curID, name), value);
	}
	void ShaderProgram::setFloat(const char* name, const float value) const {
		glUniform1f(glGetUniformLocation(curID, name), value);
	}
	void ShaderProgram::setVector3(const char* name, const glm::vec3 value) const {
		glUniform3f(glGetUniformLocation(curID, name), value.x, value.y, value.z);
	}

}