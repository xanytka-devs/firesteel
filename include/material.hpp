#include <../engine/include/shader.hpp>
#include <../engine/include/texture.hpp>
#include <../engine/include/utils/utils.hpp>
#include "utils.hpp"
using namespace Firesteel;

struct Material {
	Shader shader;
	std::string path = "";
	std::string name = "New Material";
    unsigned int type = 0;
	Material load(std::string tPath) {
		if(!std::filesystem::exists(tPath)) {
            LOG_ERRR("Material at \""+tPath+"\" doesn't exist.");
            return *this;
        }
		path = tPath;
		std::ifstream ifs(tPath);
		nlohmann::json txt = nlohmann::json::parse(ifs);
		name = txt["name"];
		std::string vP = txt["shader"]["vert"];
		std::string fP = txt["shader"]["frag"];
        type = txt["type"];
		if(!txt["shader"]["geom"].is_null()) {
			std::string gP = txt["shader"]["geom"];
			shader = Shader(vP.c_str(), fP.c_str(), gP.c_str());
		} else shader = Shader(vP.c_str(), fP.c_str());
        if(!txt["cfg"].is_null()) {
            shader.enable();
            for (auto it = txt["cfg"].begin(); it != txt["cfg"].end(); ++it) {
                if(it->is_array()) {
                    unsigned int i=0;
                    glm::vec4 vec = Vec4FromJson(it);
                    if (it->size() == 4)
                        shader.setVec4(it.key().c_str(), vec);
                    else if(it->size() == 3)
                        shader.setVec3(it.key().c_str(), vec);
                    else if(it->size() == 2)
                        shader.setVec2(it.key().c_str(), vec);
                } else if(it->is_boolean()) shader.setBool(it.key().c_str(), *it);
                else if(it->is_number_integer()) shader.setInt(it.key().c_str(), *it);
                else if(it->is_number_float()) shader.setFloat(it.key().c_str(), *it);
            }
        }
		return *this;
	}

	void reload() {
		if(shader.loaded) shader.remove();
		load(path);
	}
	void enable() const { shader.enable(); }
	bool isLoaded() const { return shader.loaded; }
    void remove() const { shader.remove(); }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(shader.ID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(shader.ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(shader.ID, name.c_str()), x, y);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(shader.ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(shader.ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(shader.ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(shader.ID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(shader.ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(shader.ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

};