#ifndef OG_SHADER_H
#define OG_SHADER_H

#include <sstream>

#include "xengine/log.hpp"
#include "xengine/math.hpp"

namespace XEngine {
	class Shader {
	public:
		unsigned int id;

		Shader();
		Shader(const char* t_vertShaderPath, const char* t_fragShaderPath);
		void enable();
		void generate(const char* t_vertShaderPath, const char* t_fragShaderPath);
		unsigned int compile(const char* t_path, unsigned int t_type);
		void remove();

		void set_bool(const std::string& t_name, bool t_val);
		void set_int(const std::string& t_name, int t_val);
		void set_float(const std::string& t_name, float t_val);
		void set_3_floats(const std::string& t_name, float t_val1, float t_val2, float t_val3);
		void set_3_floats(const std::string& t_name, glm::vec3 t_vec);
		void set_4_floats(const std::string& t_name, float t_val1, float t_val2, float t_val3, float t_val4);
		void set_4_floats(const std::string& t_name, glm::vec4 t_vec);
		void set_mat4(const std::string& t_name, glm::mat4 t_mat);
		void set_mat3(const std::string& t_name, glm::mat3 t_mat);
	};
}

#endif // OG_SHADER_H