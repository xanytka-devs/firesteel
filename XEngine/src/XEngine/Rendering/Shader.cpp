#include "XEngine/Rendering/Shader.hpp"

namespace XEngine {

    Shader::Shader(const char* t_vertShaderPath, const char* t_fragShaderPath) {
        int success = 0;
        char infoLog[512];
        //Compile vertex shader.
        GLuint vertShader = compile(t_vertShaderPath, GL_VERTEX_SHADER);
        GLuint fragShader = compile(t_fragShaderPath, GL_FRAGMENT_SHADER);
        //Create program.
        id = glCreateProgram();
        glAttachShader(id, vertShader);
        glAttachShader(id, fragShader);
        glLinkProgram(id);
        //Catch errors.
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            LOG_ERRR(infoLog);
        }
        //Delete shaders.
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    void Shader::enable() {
        glUseProgram(id); //Uses program.
    }

    GLuint Shader::compile(const char* t_path, GLenum t_type) {
        int success = 0;
        char infoLog[512];
        //Create shader and read file.
        GLuint output = glCreateShader(t_type);
        std::string shaderSrc = ResManager::read_from_file(t_path);
        const GLchar* shaderGL = shaderSrc.c_str();
        //Compile.
        glShaderSource(output, 1, &shaderGL, NULL);
        glCompileShader(output);
        //Catch errors.
        glGetShaderiv(output, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(output, 512, NULL, infoLog);
            LOG_ERRR(infoLog);
        }
        return output;
    }

    /// <summary>
    /// Set shader's 'Matrix 4' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_mat">Matrix.</param>
    void Shader::setMat4(const std::string& t_name, glm::mat4 t_mat) {
        glUniformMatrix4fv(glGetUniformLocation(id, t_name.c_str()), 1, GL_FALSE, glm::value_ptr(t_mat));
    }
    /// <summary>
    /// Set shader's 'Matrix 3' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_mat">Matrix.</param>
    void Shader::setMat3(const std::string& t_name, glm::mat3 t_mat) {
        glUniformMatrix3fv(glGetUniformLocation(id, t_name.c_str()), 1, GL_FALSE, glm::value_ptr(t_mat));
    }

    /// <summary>
    /// Set shader's 'bool' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::setBool(const std::string& t_name, bool t_val) {
        glUniform1i(glGetUniformLocation(id, t_name.c_str()), (int)t_val);
    }
    /// <summary>
    /// Set shader's 'int' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::setInt(const std::string& t_name, int t_val) {
        glUniform1i(glGetUniformLocation(id, t_name.c_str()), t_val);
    }
    /// <summary>
    /// Set shader's 'float' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::setFloat(const std::string& t_name, float t_val) {
        glUniform1f(glGetUniformLocation(id, t_name.c_str()), t_val);
    }
    /// <summary>
    /// Set 3 of shader's 'float' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::set3Floats(const std::string& t_name, float t_val1, float t_val2, float t_val3) {
        glUniform3f(glGetUniformLocation(id, t_name.c_str()), t_val1, t_val2, t_val3);
    }
    /// <summary>
    /// Set 3 of shader's 'float' variable by 'Vector 3'.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::set3Floats(const std::string& t_name, glm::vec3 t_vec) {
        glUniform3f(glGetUniformLocation(id, t_name.c_str()), t_vec.x, t_vec.y, t_vec.z);
    }
    /// <summary>
    /// Set 4 of shader's 'float' variable.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::set4Floats(const std::string& t_name, float t_val1, float t_val2, float t_val3, float t_val4) {
        glUniform4f(glGetUniformLocation(id, t_name.c_str()), t_val1, t_val2, t_val3, t_val4);
    }
    /// <summary>
    /// Set 4 of shader's 'float' variable by 'Vector 4'.
    /// </summary>
    /// <param name="t_name">Name of the variable.</param>
    /// <param name="t_val">Value.</param>
    void Shader::set4Floats(const std::string& t_name, glm::vec4 t_vec) {
        glUniform4f(glGetUniformLocation(id, t_name.c_str()), t_vec.x, t_vec.y, t_vec.z, t_vec.w);
    }

}