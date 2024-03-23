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
		/// <summary>
		/// Creates new program.
		/// </summary>
		/// <param name="t_vert_shader_path">Vertex shader path.</param>
		/// <param name="t_frag_shader_path">Fragment shader path.</param>
		/// <param name="t_load">Load shader as it's initialized.</param>
		Shader(const char* t_vert_shader_path, const char* t_frag_shader_path, bool t_load = true);
		/// <summary>
		/// Set program as current.
		/// </summary>
		void enable();
		/// <summary>
		/// Disable current shader.
		/// </summary>
		void disable();
		/// <summary>
		/// Generates and links vertex and fragment sahders.
		/// </summary>
		/// <param name="t_vert_shader_path">Vertex shader path.</param>
		/// <param name="t_frag_shader_path">Fragment shader path.</param>
		void generate(const char* t_vert_shader_path, const char* t_frag_shader_path);
		/// <summary>
		/// Compilies shader.
		/// </summary>
		/// <param name="t_path">Path to shader.</param>
		/// <param name="t_type">Shader type.</param>
		/// <returns>Compilied shader.</returns>
		unsigned int compile(const char* t_path, unsigned int t_type);
		/// <summary>
		/// Deletes program (cleanup).
		/// </summary>
		void remove();

		/// <summary>
		/// Set shader's 'bool' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val">Value.</param>
		void set_bool(std::string t_name, bool t_val);
		/// <summary>
		/// Set shader's 'int' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val">Value.</param>
		void set_int(std::string t_name, int t_val);
		/// <summary>
		/// Set shader's 'float' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val">Value.</param>
		void set_float(std::string t_name, float t_val);
		/// <summary>
		/// Set 2 of shader's 'float' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val1">First value.</param>
		/// <param name="t_val2">Second value.</param>
		void set_2_floats(std::string t_name, float t_val1, float t_val2);
		/// <summary>
		/// Set 3 of shader's 'float' variable by 'Vector 2'.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_vec">Value.</param>
		void set_2_floats(std::string t_name, glm::vec2 t_vec);
		/// <summary>
		/// Set 3 of shader's 'float' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val1">First value.</param>
		/// <param name="t_val2">Second value.</param>
		/// <param name="t_val3">Third value.</param>
		void set_3_floats(std::string t_name, float t_val1, float t_val2, float t_val3);
		/// <summary>
		/// Set 3 of shader's 'float' variable by 'Vector 3'.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_vec">Value.</param>
		void set_3_floats(std::string t_name, glm::vec3 t_vec);
		/// <summary>
		/// Set 4 of shader's 'float' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_val1">First value.</param>
		/// <param name="t_val2">Second value.</param>
		/// <param name="t_val3">Third value.</param>
		/// <param name="t_val4">Fourth value.</param>
		void set_4_floats(std::string t_name, float t_val1, float t_val2, float t_val3, float t_val4);
		/// <summary>
		/// Set 4 of shader's 'float' variable by 'Vector 4'.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_vec">Value.</param>
		void set_4_floats(std::string t_name, glm::vec4 t_vec);
		/// <summary>
		/// Set shader's 'Matrix 3' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_mat">Matrix.</param>
		void set_mat3(std::string t_name, glm::mat3 t_mat);
		/// <summary>
		/// Set shader's 'Matrix 4' variable.
		/// </summary>
		/// <param name="t_name">Name of the variable.</param>
		/// <param name="t_mat">Matrix.</param>
		void set_mat4(std::string t_name, glm::mat4 t_mat);
	};
}

#endif // OG_SHADER_H