#ifndef FS_MATERIAL
#define FS_MATERIAL

#include <variant>

#include <firesteel/common.hpp>
#include <firesteel/rendering/shader.hpp>
#include <firesteel/rendering/texture.hpp>
#ifndef FS_NO_JSON
#include <firesteel/utils/json.hpp>
#endif // !FS_NO_JSON

#define SAMETYPE(type, base) std::is_same_v<type,base>

namespace Firesteel {
    //All available shader parameter types.
    //Can hold values for one or all types at the same time.
    //To get value from variant holder(H) for surtain type(T) use `std::get<T>(H);`.
    using ShaderParameterValue = std::variant<
        bool,int,float,glm::vec2,glm::vec3,glm::vec4,glm::mat2,glm::mat3,glm::mat4
    >;
    enum ShaderParameterValueType {
        SPVT_UNKNOWN,
        SPVT_BOOL,
        SPVT_INT,
        SPVT_FLOAT,
        SPVT_VEC2,
        SPVT_VEC3,
        SPVT_VEC4
    };

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
        std::string typeString() const {
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
        ShaderParameterValueType type() const {
            return std::visit([](const auto& value) -> ShaderParameterValueType {
                //Get type of value, that is being held in parameter.
                using T = std::decay_t<decltype(value)>;
                //Try to return string of saved typename.
                IFEX(SAMETYPE(T, bool))      return SPVT_BOOL;
                IFEX(SAMETYPE(T, int))       return SPVT_INT;
                IFEX(SAMETYPE(T, float))     return SPVT_FLOAT;
                IFEX(SAMETYPE(T, glm::vec2)) return SPVT_VEC2;
                IFEX(SAMETYPE(T, glm::vec3)) return SPVT_VEC3;
                IFEX(SAMETYPE(T, glm::vec4)) return SPVT_VEC4;
                IFEX(SAMETYPE(T, glm::mat2)) return SPVT_VEC2;
                IFEX(SAMETYPE(T, glm::mat3)) return SPVT_VEC3;
                IFEX(SAMETYPE(T, glm::mat4)) return SPVT_VEC4;
                return SPVT_UNKNOWN;
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
        Material(const std::string& tVertexPath, const std::string& tFragmentPath, const std::string& tGeometryPath="") {
            setShader(tVertexPath,tFragmentPath,tGeometryPath);
        }

#ifndef FS_NO_JSON
        // Loads material from specified json file.
        void load(const std::string& tMaterialPath) {
            if(!std::filesystem::exists(tMaterialPath)) {
                LOG_ERROR("Couldn't open material file at \""+tMaterialPath+"\"");
                return;
            }
            //Load file contents.
            std::ifstream file;
            std::stringstream fileStream;
            try {
                file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                file.open(tMaterialPath);
                fileStream << file.rdbuf();
                file.close();
            }
            catch (std::ifstream::failure& e) {
                LOGF_WARN("Error while reading material file at: %s", e.what());
                return;
            }
            //Parse material file.
            nlohmann::json txt=nlohmann::json::parse(fileStream.str());
            name=txt["name"];
            type=txt["type"];
            //Get shader.
            vertexPath=txt["shader"]["vert"];
            fragmentPath=txt["shader"]["frag"];
            if(!txt["shader"]["geom"].is_null()) {
                geometryPath=txt["shader"]["geom"];
                setShader(std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath));
            } else setShader(std::make_shared<Shader>(vertexPath, fragmentPath));
            //Load parameters.
            if(!txt["param"].is_null() && mShader->loaded()) {
                mShader->enable();
                for(auto it = txt["param"].begin(); it!=txt["param"].end(); ++it) {
                    if(it->is_array()) {
                        //Parse array.
                        glm::vec4 vec{};
                        unsigned int i=0;
                        for(auto v=it->begin();v!=it->end();++v) {
                            vec[i]=v.value();
                            i++;
                        }
                        //Place param.
                        if(it->size()==4) params.emplace_back(it.key().c_str(),glm::vec4(vec.x,vec.y,vec.z,vec.w));
                        else if(it->size()==3) params.emplace_back(it.key().c_str(),glm::vec3(vec.x,vec.y,vec.z));
                        else if(it->size()==2) params.emplace_back(it.key().c_str(),glm::vec2(vec.x,vec.y));
                    } else if(it->is_boolean()) {
                        params.emplace_back(it.key().c_str(),it.value().get<bool>());
                    } else if (it->is_number_integer()) {
                        params.emplace_back(it.key().c_str(),it.value().get<int>());
                    } else if(it->is_number_float()) {
                        params.emplace_back(it.key().c_str(),it.value().get<float>());
                    }
                }
            }
        }
        // Saves material to specified json file.
        bool save(const std::string& tMaterialPath) {
            nlohmann::json txt;
            //Basic properties.
            txt["name"]=name;
            txt["type"]=type;
            txt["shader"]["vert"]=vertexPath;
            txt["shader"]["frag"]=fragmentPath;
            if(!geometryPath.empty()) txt["shader"]["frag"]=geometryPath;
            //Save parameters.
            for(size_t i=0;i<params.size();i++) {
                switch(params[i].type()) {
                case SPVT_VEC2:
                    txt["param"][params[i].name()][0]=params[i].get<glm::vec2>().x;
                    txt["param"][params[i].name()][1]=params[i].get<glm::vec2>().y;
                    break;
                case SPVT_VEC3:
                    txt["param"][params[i].name()][0]=params[i].get<glm::vec3>().x;
                    txt["param"][params[i].name()][1]=params[i].get<glm::vec3>().y;
                    txt["param"][params[i].name()][2]=params[i].get<glm::vec3>().z;
                    break;
                case SPVT_VEC4:
                    txt["param"][params[i].name()][0]=params[i].get<glm::vec4>().x;
                    txt["param"][params[i].name()][1]=params[i].get<glm::vec4>().y;
                    txt["param"][params[i].name()][2]=params[i].get<glm::vec4>().z;
                    txt["param"][params[i].name()][3]=params[i].get<glm::vec4>().w;
                    break;
                case SPVT_BOOL:
                    txt["param"][params[i].name()]=params[i].get<bool>();
                    break;
                case SPVT_INT:
                    txt["param"][params[i].name()]=params[i].get<int>();
                    break;
                case SPVT_FLOAT:
                    txt["param"][params[i].name()]=params[i].get<float>();
                    break;
                default:
                    LOGF_WARN("Unknown parameter \"%s\" for material \"%s\"",params[i].name(),name.c_str());
                    break;
                }
            }

            std::ofstream o(tMaterialPath);
            o << std::setw(4) << txt << std::endl;
            return true;
        }

        void reload() {

        }
#endif // !FS_NO_JSON

        void setShader(const std::string& tVertexPath, const std::string& tFragmentPath, const std::string& tGeometryPath="") {
            mShader=std::make_shared<Shader>(tVertexPath, tFragmentPath, tGeometryPath);
            if(!mShader) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (pointer was empty)");
            } else if(!mShader->loaded()) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (shader wasn't loaded)");
            }
        }
        void setShader(std::shared_ptr<Shader>& tShader) {
            mShader=tShader;
            if(!mShader) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (pointer was empty)");
            } else if(!mShader->loaded()) {
                mShader=Shader::getDefaultShader();
                LOG_ERRR("Failed to set shader (shader wasn't loaded)");
            }
        }
        std::shared_ptr<Shader> getShader() const {return mShader;}
        bool loaded() const {
            if(!mShader) return false;
            return mShader->loaded();
        }

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
    public:
        std::vector<ShaderParameter> params;
        std::vector<Texture> textures;
#ifndef FS_NO_JSON
        std::string vertexPath="";
        std::string fragmentPath="";
        std::string geometryPath="";
        std::string path="";
        std::string name="New Material";
        unsigned int type=0;
#endif // !FS_NO_JSON
    private:
        std::shared_ptr<Shader> mShader=Shader::getDefaultShader();
    };
}

#endif // !FS_MATERIAL