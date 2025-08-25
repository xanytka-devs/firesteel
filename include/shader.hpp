#ifndef FS_SHADER_H
#define FS_SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "common.hpp"

namespace Firesteel {
    struct Shader {
    private:
        static Shader sDefaultShader;
    public:
        unsigned int ID;
        bool loaded=false;

        Shader() {
            remove();
            ID=0;
        }
        Shader(const char* tVertexPath, const char* tFragmentPath, const char* tGeometryPath=nullptr) {
            remove();
            //Retrieve the vertex/fragment source code from file path.
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;
            //Check if given paths even exist.
            bool hasGeomShader=(tGeometryPath != nullptr);
            if(!std::filesystem::exists(tVertexPath)) {
                LOG_WARN(std::string("Vertex shader at \"") + tVertexPath + "\" doesn't exist");
                ID=sDefaultShader.ID;
                return;
            }
            if(!std::filesystem::exists(tFragmentPath)) {
                LOG_WARN(std::string("Fragment shader at \"") + tFragmentPath + "\" doesn't exist");
                ID=sDefaultShader.ID;
                return;
            }
            if(hasGeomShader)
                if(!std::filesystem::exists(tGeometryPath)) {
                    LOG_WARN(std::string("Geometry shader at \"") + tGeometryPath + "\" doesn't exist");
                    hasGeomShader=false;
                }
            //Ensure ifstream objects can throw exceptions:.
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            if(hasGeomShader) gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                //Open files.
                vShaderFile.open(tVertexPath);
                fShaderFile.open(tFragmentPath);
                if(hasGeomShader) gShaderFile.open(tFragmentPath);
                std::stringstream vShaderStream, fShaderStream, gShaderStream;
                //Read file's buffer contents into streams.
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                if(hasGeomShader) gShaderStream << gShaderFile.rdbuf();
                //Close file handlers.
                vShaderFile.close();
                fShaderFile.close();
                if(hasGeomShader) gShaderFile.close();
                //Convert stream into string.
                vertexCode=vShaderStream.str();
                fragmentCode=fShaderStream.str();
                if(hasGeomShader) geometryCode=gShaderStream.str();
                //Generate the program itself.
                generate(vertexCode.c_str(), fragmentCode.c_str(), hasGeomShader, geometryCode.c_str());
            }
            catch (std::ifstream::failure& e) {
                LOG_WARN(std::string("Error while reading shader files: ") + e.what());
                ID=sDefaultShader.ID;
                return;
            }
        }
        Shader(const char* tVertexCode, const char* tFragmentCode, const bool& tHasGeomShader, const char* tGeometryCode) {
            remove();
            generate(tVertexCode, tFragmentCode, tHasGeomShader, tGeometryCode);
        }

        void bind() const {
            if(!loaded) return;
            glUseProgram(ID);
            setFloat("time", static_cast<float>(glfwGetTime()));
        }
        //Alias for `bind()`.
        void enable() const {bind();}
        void remove() {
            if(loaded) {
                glDeleteProgram(ID);
                loaded=false;
            }
        }

        static void setDefaultShader(const char* tVertexCode, const char* tFragmentCode) {
            sDefaultShader.remove();
            sDefaultShader=Shader(tVertexCode,tFragmentCode,false,nullptr);
        }
        static Shader* getDefaultShader() {
            return &sDefaultShader;
        }

        // Utilities //

        void setInt(const std::string& tName, const int tValue) const {
            glUniform1i(glGetUniformLocation(ID, tName.c_str()), tValue);
        }
        void setFloat(const std::string& tName, const float tValue) const {
            glUniform1f(glGetUniformLocation(ID, tName.c_str()), tValue);
        }
        void setVec2(const std::string& tName, const float tX, const float tY) const {
            glUniform2f(glGetUniformLocation(ID, tName.c_str()), tX, tY);
        }
        void setVec3(const std::string& tName, const float tX, const float tY, const float tZ) const {
            glUniform3f(glGetUniformLocation(ID, tName.c_str()), tX, tY, tZ);
        }
        void setVec4(const std::string& tName, const float tX, const float tY, const float tZ, const float tW) const {
            glUniform4f(glGetUniformLocation(ID, tName.c_str()), tX, tY, tZ, tW);
        }
        void setMat2(const std::string& tName, const glm::mat2& tMat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, tName.c_str()), 1, GL_FALSE, &tMat[0][0]);
        }
        void setMat3(const std::string& tName, const glm::mat3& tMat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, tName.c_str()), 1, GL_FALSE, &tMat[0][0]);
        }
        void setMat4(const std::string& tName, const glm::mat4& tMat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, tName.c_str()), 1, GL_FALSE, &tMat[0][0]);
        }

        void setBool(const std::string& tName, const bool tValue) const {
            setInt(tName, (int)tValue);
        }        
        void setVec2(const std::string& tName, const glm::vec2& tValue) const {
            setVec2(tName, tValue.x, tValue.y);
        }        
        void setVec3(const std::string& tName, const glm::vec3& tValue) const {
            setVec3(tName, tValue.x, tValue.y, tValue.z);
        }        
        void setVec4(const std::string& tName, const glm::vec4& tValue) const {
            setVec4(tName, tValue.x, tValue.y, tValue.z, tValue.w);
        }

    private:
        bool getErrors(GLuint tShader, size_t tType=0) {
            GLint success;
            GLchar infoLog[1024];
            if (tType != 1) {
                glGetShaderiv(tShader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(tShader, 1024, NULL, infoLog);
                    LOG_ERRR(std::string("Shader compilation error:\n") + infoLog);
                }
            }
            else {
                glGetProgramiv(tShader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(tShader, 1024, NULL, infoLog);
                    LOG_ERRR(std::string("Program compilation error:\n") + infoLog);
                }
            }
            return success;
        }
        void generate(const char* tVShaderCode, const char* tFShaderCode, const bool& tHasGeomShader, const char* tGShaderCode) {
            unsigned int vertex, fragment, geometry;
            //Vertex shader.
            vertex=glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &tVShaderCode, NULL);
            glCompileShader(vertex);
            if(!getErrors(vertex)) return;
            //Fragment Shader.
            fragment=glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &tFShaderCode, NULL);
            glCompileShader(fragment);
            if (!getErrors(fragment)) return;
            //Geometry Shader.
            if (tHasGeomShader) {
                geometry=glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &tGShaderCode, NULL);
                glCompileShader(geometry);
                if (!getErrors(geometry)) return;
            }
            //Link program.
            ID=glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            if (tHasGeomShader) glAttachShader(ID, geometry);
            glLinkProgram(ID);
            getErrors(ID, 1);
            //Delete unnecessary shaders.
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            if(tHasGeomShader) glDeleteShader(geometry);
            loaded=true;
        }
    };
}
#endif // !FS_SHADER_H