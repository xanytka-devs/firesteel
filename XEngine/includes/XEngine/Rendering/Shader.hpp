#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "XEngine/ResManager.hpp"
#include "XEngine/Log.hpp"

namespace XEngine {
	class Shader {
	public:
		unsigned int id;

		Shader(const char* t_vertShaderPath, const char* t_fragShaderPath);
		void enable();
		GLuint compile(const char* t_path, GLenum t_type);

		void setBool(const std::string& t_name, bool t_val);
		void setInt(const std::string& t_name, int t_val);
		void setFloat(const std::string& t_name, float t_val);
		void set3Floats(const std::string& t_name, float t_val1, float t_val2, float t_val3);
		void set3Floats(const std::string& t_name, glm::vec3 t_vec);
		void set4Floats(const std::string& t_name, float t_val1, float t_val2, float t_val3, float t_val4);
		void set4Floats(const std::string& t_name, glm::vec4 t_vec);
		void setMat4(const std::string& t_name, glm::mat4 t_mat);
		void setMat3(const std::string& t_name, glm::mat3 t_mat);
	};
}

#endif //SHADER_H