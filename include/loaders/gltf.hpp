#ifndef FS_LOADERS_GLTF
#define FS_LOADERS_GLTF

#include "../model.hpp"
#include "../utils/json.hpp"
#include "../utils/stbi_global.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_JSON
#define JSON_NOEXCEPTION
#include "../../external/tiny_gltf.h"

namespace Firesteel {
    namespace GLTF {
        /// [!WARING]
        /// This function is internal and only used for the OBJ loader. Use it at your own risk.
        Texture loadMaterialTexture(const Model* tModel, const std::string& texPath, const std::string& type) {
            if(texPath.empty()) return {};
            //Get full path.
            std::string fullPath = tModel->getDirectory() + "/" + texPath;
            //Check all materials for this texture.
            for (auto& material : tModel->materials) {
                for (const auto& texture : material.textures) {
                    LOG(texture.path+" "+texPath+" "+texture.type+" "+type);
                    if (texture.path == texPath && texture.type == type) {
                        //If texture is already loaded - return it's copy.
#ifdef FS_PRINT_DEBUG_MSGS
                        LOG_DBG("Texture \"" + texPath + "\" is already loaded");
#endif
                        return texture;
                    }
                }
            }
            //Otherwise load the texture.
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Loading texture \"" + texPath + "\"");
#endif
            Texture texture;
            texture.type = type;
            texture.path = texPath;
            texture.ID = TextureFromFile(fullPath.c_str(), &texture.isMonochrome, true);

            return texture;
        }

        /// [!WARING]
        /// This function is internal and only used for the OBJ loader. Use it at your own risk.
        Mesh processPrimitive(const Model* tBaseModel, const tinygltf::Model& tModel, const tinygltf::Primitive& tPrimitive, const std::string tPath
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex) {
#else
            ) {
#endif
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Positions.
            const tinygltf::Accessor& posAcc=tModel.accessors[tPrimitive.attributes.at("POSITION")];
            const tinygltf::BufferView& posView=tModel.bufferViews[posAcc.bufferView];
            const float* positions=reinterpret_cast<const float*>(&tModel.buffers[posView.buffer].data[posView.byteOffset]);
            //Normals.
            const tinygltf::Accessor& normAcc=tModel.accessors[tPrimitive.attributes.at("NORMAL")];
            const tinygltf::BufferView& normView=tModel.bufferViews[normAcc.bufferView];
            const float* normals=reinterpret_cast<const float*>(&tModel.buffers[normView.buffer].data[normView.byteOffset]);
            //UVs.
            const tinygltf::Accessor& uvAcc=tModel.accessors[tPrimitive.attributes.at("TEXCOORD_0")];
            const tinygltf::BufferView& uvView=tModel.bufferViews[uvAcc.bufferView];
            const float* uvs=reinterpret_cast<const float*>(&tModel.buffers[uvView.buffer].data[uvView.byteOffset]);
            //Create vertexes.
            for(size_t i=0;i<posAcc.count;i++) {
                Vertex vert{};
                vert.position=glm::vec3(positions[0]+i*3,positions[1]+i*3,positions[2]+i*3);
                vert.normal=glm::vec3(normals[0]+i*3,normals[1]+i*3,normals[2]+i*3);
                vert.uv=glm::vec2(uvs[0]+i*2,uvs[1]+i*2);
                vertices.push_back(vert);
#ifdef FS_PRINT_DEBUG_MSGS
                tVert += 3;
                tNorm += 3;
                tTex += 2;
#endif
            }
            //Create indicies.
            const tinygltf::Accessor& indAcc=tModel.accessors[tPrimitive.indices];
            const tinygltf::BufferView& indView=tModel.bufferViews[indAcc.bufferView];
            const float* indData=reinterpret_cast<const float*>(&tModel.buffers[indView.buffer].data[indView.byteOffset]);
            indices.resize(indAcc.count);
            switch (indAcc.componentType)
            {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                const uint8_t* data8=reinterpret_cast<const uint8_t*>(indData);
                for(size_t i=0;i<indAcc.count;i++)
                    indices[i]=static_cast<unsigned int>(data8[i]);
                break;
            }
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                const uint16_t* data16=reinterpret_cast<const uint16_t*>(indData);
                for(size_t i=0;i<indAcc.count;i++)
                    indices[i]=static_cast<unsigned int>(data16[i]);
                break;
            }
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                const uint32_t* data32=reinterpret_cast<const uint32_t*>(indData);
                memcpy(indices.data(),data32,indAcc.count*sizeof(unsigned int));
                break;
            }
            default:
                LOG_ERRR("Unsupported index component type");
                break;
            }
            //Get textures.
            std::vector<Texture> textures;
            const tinygltf::Material& material=tModel.materials[tPrimitive.material];
            if(material.pbrMetallicRoughness.baseColorTexture.index>=0) {
                const tinygltf::Texture& tex=tModel.textures[material.pbrMetallicRoughness.baseColorTexture.index];
                const tinygltf::Image& img=tModel.images[tex.source];
                textures.push_back(loadMaterialTexture(tBaseModel,img.uri,"diffuse"));
            }

            return Mesh(vertices,indices,textures);
        }

        Model load(std::string tPath, bool tBinary) {
            Model model{ tPath };

            tinygltf::Model gltf;
            tinygltf::TinyGLTF loader;
            std::string error, warn;

            bool res = tBinary?loader.LoadBinaryFromFile(&gltf,&error,&warn,tPath):loader.LoadASCIIFromFile(&gltf,&error,&warn,tPath);
            if(!error.empty())
                LOG_ERRR("Got error while loading model: " + error + "(at \"" + tPath + "\")");
            if(!warn.empty())
                LOG_ERRR("Got warning while loading model: " + warn + "(at \"" + tPath + "\")");
            if(!res) {
                LOG_ERRR("Failed to load model");
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Materials: "+std::to_string((int)gltf.materials.size()));
            LOG_DBG("Meshes: "+std::to_string((int)gltf.meshes.size()));
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            for(size_t m=0;m<gltf.meshes.size();m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOG_DBG("Processing mesh "+std::to_string((int)(m+1))+"/"+std::to_string((int)gltf.meshes.size()));
#endif // FS_PRINT_DEBUG_MSGS
                for(size_t p=0;p<gltf.meshes[m].primitives.size();p++) {
#ifdef FS_PRINT_DEBUG_MSGS
                    LOG_DBG("Processing premitive "+std::to_string((int)(p+1))+"/"+std::to_string((int)gltf.meshes[m].primitives.size()));
                    model.meshes.push_back(processPrimitive(&model,gltf,gltf.meshes[m].primitives[p],model.getDirectory(),vert,ind,norm,tex));
#else
                    model.meshes.push_back(processPrimitive(&model,gltf,gltf.meshes[m].primitives[p],model.getDirectory()));
#endif // FS_PRINT_DEBUG_MSGS
                }
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Vertices: "+std::to_string((int)(vert) / 3));
            LOG_DBG("Normals: "+std::to_string((int)(norm) / 3));
            LOG_DBG("UVs: "+std::to_string((int)(tex) / 2));
#endif // FS_PRINT_DEBUG_MSGS
            
            return model;
        }
    }
}

#endif // !FS_LOADERS_GLTF