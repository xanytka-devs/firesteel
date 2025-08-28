#ifndef FS_MATERIAL
#define FS_MATERIAL

#include <variant>

#include "common.hpp"
#include "shader.hpp"
#include "texture.hpp"

#define SAMETYPE(type, base) std::is_same_v<type,base>

namespace Firesteel {
    //All available shader parameter types.
    //Can hold values for one or all types at the same time.
    //To get value from variant holder(H) for surtain type(T) use `std::get<T>(H);`.
    using ShaderParameterValue = std::variant<
        bool,int,float,glm::vec2,glm::vec3,glm::vec4,glm::mat2,glm::mat3,glm::mat4
    >;

    class ShaderParameter {
    public:
        ShaderParameter(const char* tParam, const ShaderParameterValue& tValue) : mName(tParam), mValue(tValue) {}
        void set(const ShaderParameterValue& tValue) {mValue=tValue;}
        template<typename T>
        T get() const {
            T value;
            try {
                value = std::get<T>(mValue);
            } catch(const std::bad_variant_access& e) {
                LOGF_ERRR("Tried to request invalid typename from ShaderParameter::getValue(): %s", e.what());
                return value;
            }
        }
        void bind(const Shader* tShader) const {
            std::visit([this,tShader](const auto& value) {
                //Get type of value, that is being held in parameter.
                using T = std::decay_t<decltype(mValue)>;
                //Try to bind saved typename to the parameter name given.
                IFEX(SAMETYPE(T,bool))      tShader->setBool(mName,value);
                IFEX(SAMETYPE(T,int))       tShader->setInt(mName,value);
                IFEX(SAMETYPE(T,float))     tShader->setFloat(mName,value);
                IFEX(SAMETYPE(T,glm::vec2)) tShader->setVec2(mName,value);
                IFEX(SAMETYPE(T,glm::vec3)) tShader->setVec3(mName,value);
                IFEX(SAMETYPE(T,glm::vec4)) tShader->setVec4(mName,value);
                IFEX(SAMETYPE(T,glm::mat2)) tShader->setMat2(mName,value);
                IFEX(SAMETYPE(T,glm::mat3)) tShader->setMat3(mName,value);
                IFEX(SAMETYPE(T,glm::mat4)) tShader->setMat4(mName,value);
            }, mValue);
        }
        std::string type() const {
            std::visit([this](const auto& value) {
                //Get type of value, that is being held in parameter.
                using T = std::decay_t<decltype(mValue)>;
                //Try to return string of saved typename.
                IFEX(SAMETYPE(T,bool))      return "bool";
                IFEX(SAMETYPE(T,int))       return "int";
                IFEX(SAMETYPE(T,float))     return "float";
                IFEX(SAMETYPE(T,glm::vec2)) return "glm::vec2";
                IFEX(SAMETYPE(T,glm::vec3)) return "glm::vec3";
                IFEX(SAMETYPE(T,glm::vec4)) return "glm::vec4";
                IFEX(SAMETYPE(T,glm::mat2)) return "glm::mat2";
                IFEX(SAMETYPE(T,glm::mat3)) return "glm::mat3";
                IFEX(SAMETYPE(T,glm::mat4)) return "glm::mat4";
            }, mValue);
        }
        const std::string& name() const {return mName;}
    protected:
        const char* mName;
        ShaderParameterValue mValue;
    };
    struct Material {
        Shader* shader=nullptr;
        std::vector<ShaderParameter> params;
        std::vector<Texture> textures;
        void bind() {
            if(!shader) return;
            shader->enable();
            for (size_t p=0;p<params.size();p++)
                params[p].bind(shader);
            
        }
        void remove() {
            for(size_t i=0; i < textures.size(); i++)
                textures[i].remove();
            if(shader) shader->remove();
        }
    };
}

#endif // !FS_MATERIAL