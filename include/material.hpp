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
                using T = std::decay_t<decltype(value)>;
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
            return std::visit([](const auto& value) -> std::string {
                //Get type of value, that is being held in parameter.
                using T = std::decay_t<decltype(value)>;
                //Try to return string of saved typename.
                IFEX(SAMETYPE(T,bool))      return "bool";
                IFEX(SAMETYPE(T,int))       return "int";
                IFEX(SAMETYPE(T,float))     return "float";
                IFEX(SAMETYPE(T,glm::vec2)) return "vec2";
                IFEX(SAMETYPE(T,glm::vec3)) return "vec3";
                IFEX(SAMETYPE(T,glm::vec4)) return "vec4";
                IFEX(SAMETYPE(T,glm::mat2)) return "mat2";
                IFEX(SAMETYPE(T,glm::mat3)) return "mat3";
                IFEX(SAMETYPE(T,glm::mat4)) return "mat4";
                return "unknown";
            }, mValue);
        }
        const char* name() const {return mName;}
    protected:
        const char* mName;
        ShaderParameterValue mValue;
    };
    struct Material {
    public:
        Material() {}
        Material(std::shared_ptr<Shader>& tShader) {
            setShader(tShader);
        }
        Material(const char* tVertexPath, const char* tFragmentPath, const char* tGeometryPath = nullptr) {
            setShader(tVertexPath,tFragmentPath,tGeometryPath);
        }
        void setShader(const char* tVertexPath, const char* tFragmentPath, const char* tGeometryPath = nullptr) {
            mShader=std::make_shared<Shader>(tVertexPath, tFragmentPath, tGeometryPath);
            if(!mShader) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (pointer was empty)");
            } else if(!mShader->loaded) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (shader wasn't loaded)");
            }
        }
        void setShader(std::shared_ptr<Shader>& tShader) {
            mShader=tShader;
            if(!mShader) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (pointer was empty)");
            } else if(!mShader->loaded) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (shader wasn't loaded)");
            }
        }
        std::shared_ptr<Shader> getShader() const {return mShader;}
        void bind() {
            if(!mShader) mShader=Shader::getDefaultShader();
            mShader->enable();
            for(size_t p=0;p<params.size();p++)
                params[p].bind(mShader.get());
            //Bind appropriate textures.
            size_t diffuseNr=0;
            size_t specularNr=0;
            size_t normalNr=0;
            size_t heightNr=0;
            size_t emisNr=0;
            size_t opacNr=0;
            mShader->setBool("noTextures", true);
            //Bind textures.
            if(textures.size()>0) {
                mShader->setBool("noTextures", false);
                Texture::unbind();
                for(unsigned int i=0; i < textures.size(); i++) {
                    //Retrieve texture number.
                    size_t number=0;
                    std::string name=textures[i].typeToString();
                    if(name == "diffuse") number=diffuseNr++;
                    else if(name == "specular") number=specularNr++;
                    else if(name == "normal") number=normalNr++;
                    else if(name == "emission") number=emisNr++;
                    else if(name == "height") number=heightNr++;
                    else if(name == "opacity") number=opacNr++;
                    //Now set the sampler to the correct texture unit.
                    textures[i].enable(i);
                    mShader->setInt("material." + name + std::to_string(number), i);
                    mShader->setBool("material." + name + std::to_string(number) + "_isMonochrome", textures[i].isMonochrome);
                    mShader->setBool("material." + name + "_present", true);
                }
                mShader->setBool("material.opacityMask", opacNr > 0);
            }
        }
        void remove() {
            for(size_t i=0; i < textures.size(); i++)
                textures[i].remove();
            if(mShader) mShader->remove();
            textures.clear();
            params.clear();
        }
    private:
        std::shared_ptr<Shader> mShader=Shader::getDefaultShader();
    public:
        std::vector<ShaderParameter> params;
        std::vector<Texture> textures;
    };
}

#endif // !FS_MATERIAL