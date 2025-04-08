#ifndef FS_SHADER_H
#define FS_SHADER_H

#include "common.hpp"

#include <string>
#include <fstream>
#include <sstream>
namespace Firesteel {
    struct Shader {
    public:
        unsigned int ID;
        bool loaded = false;

        Shader() {
            remove();
            ID = 0;
        }

        /// Constructor generates the shader on the fly.
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
            remove();
            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;
            bool hasGeomShader = (geometryPath != nullptr);
            int loadPhase = 0;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            if (hasGeomShader) gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                if (hasGeomShader) gShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream, gShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                if (hasGeomShader) gShaderStream << gShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                if (hasGeomShader) gShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
                if (hasGeomShader) geometryCode = gShaderStream.str();
                generate(vertexCode.c_str(), fragmentCode.c_str(), hasGeomShader, geometryCode.c_str());
            }
            catch (std::ifstream::failure& e) {
                LOG_WARN(std::string("Error while reading shader files: ") + e.what());
            }
        }
        
        Shader(const char* vertexCode, const char* fragmentCode, const bool& hasGeomShader, const char* geometryCode) {
            remove();
            generate(vertexCode, fragmentCode, hasGeomShader, geometryCode);
        }

        /// Activate the shader.
        void enable() const {
            glUseProgram(ID);
        }
        /// Clean up.
        void remove() const {
            if(loaded) glDeleteProgram(ID);
        }

        // Utilities //

        void setBool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void setInt(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setVec2(const std::string& name, const glm::vec2& value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }

        void setVec3(const std::string& name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }

        void setVec4(const std::string& name, const glm::vec4& value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }

        void setMat2(const std::string& name, const glm::mat2& mat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat3(const std::string& name, const glm::mat3& mat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat4(const std::string& name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        /// Get OpenGL errors.
        bool getErrors(GLuint shader, size_t type = 0) {
            GLint success;
            GLchar infoLog[1024];
            if (type != 1) {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    LOG_ERRR(std::string("Shader compilation error:\n") + infoLog);
                }
            }
            else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    LOG_ERRR(std::string("Program compilation error:\n") + infoLog);
                }
            }
            return success;
        }
        void generate(const char* vShaderCode, const char* fShaderCode, const bool& hasGeomShader, const char* gShaderCode) {
            unsigned int vertex, fragment, geometry;
            //Vertex shader.
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            if(!getErrors(vertex)) return;
            //Fragment Shader.
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            if (!getErrors(fragment)) return;
            //Geometry Shader.
            if (hasGeomShader) {
                geometry = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &gShaderCode, NULL);
                glCompileShader(geometry);
                if (!getErrors(geometry)) return;
            }
            //Link program.
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            if (hasGeomShader) glAttachShader(ID, geometry);
            glLinkProgram(ID);
            getErrors(ID, 1);
            //Delete unnecessary shaders.
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            if (hasGeomShader) glDeleteShader(geometry);
            loaded = true;
        }
    };
}
#endif // !FS_SHADER_H