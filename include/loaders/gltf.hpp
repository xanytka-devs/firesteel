#ifdef FS_LOADER_GLTF
#ifndef FS_LOADERS_GLTF_H
#define FS_LOADERS_GLTF_H

#include "../mesh.hpp"
#include "../utils/stbi_global.hpp"
#include "../utils/json.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_JSON
#define JSON_NOEXCEPTION
#include "../../external/model_loaders/tiny_gltf.h"

namespace Firesteel {
    namespace GLTF {
        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        Texture loadMaterialTexture(const Model* tModel, const std::string& texPath, const TextureType& type) {
            if(texPath.empty()) return {};
            //Get full path.
            std::string fullPath=tModel->getDirectory() + "/" + texPath;
            //Check all materials for this texture.
            for (auto& material : tModel->materials) {
                for (const auto& texture : material.textures) {
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
            texture.type=type;
            texture.path=texPath;
            texture.ID=TextureFromFile(fullPath.c_str(), &texture.isMonochrome, true);

            return texture;
        }

        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        std::string getTexPath(const tinygltf::Model* tModel, const int tId) {
            if(tId>=0) return tModel->images[tModel->textures[tId].source].uri;
            return "";
        }

        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        bool hasAttribute(const tinygltf::Primitive* tPrimitive, const char* tAttrib) {
            return tPrimitive->attributes.find(tAttrib)!=tPrimitive->attributes.end();
        }

        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        void processNodes(Model* tBaseModel, const tinygltf::Model* tModel, int tIndex, Node* tParent=nullptr) {
            const tinygltf::Node& gltfNode=tModel->nodes[tIndex];
            Node node;
            node.name=gltfNode.name.empty()?"Node_"+std::to_string(tIndex):gltfNode.name;
            glm::vec3 pos(0);
            glm::quat rot(1,0,0,0);
            glm::vec3 size(1);
            //Rescue all available transform data.
            if(!gltfNode.translation.empty()) pos=glm::make_vec3(gltfNode.translation.data());
            if(!gltfNode.rotation.empty()) rot=glm::make_quat(gltfNode.rotation.data());
            if(!gltfNode.scale.empty()) size=glm::make_vec3(gltfNode.scale.data());
            //Apply translations to node.
            node.transform.position=pos;
            node.transform.rotation=glm::degrees(glm::eulerAngles(rot));
            node.transform.size=size;
            if(gltfNode.mesh>=0) node.index=gltfNode.mesh;
            if(tParent) tParent->children.push_back(node);
            else tBaseModel->nodes.push_back(node);
            for(size_t c=0;c<gltfNode.children.size();c++)
                processNodes(tBaseModel,tModel,gltfNode.children[c],(tParent?&tParent->children.back():&tBaseModel->nodes.back()));
        }

        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        Mesh processPrimitive(Model* tBaseModel, const tinygltf::Model* tModel, const tinygltf::Primitive* tPrimitive, const std::string tPath
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Positions.
            const tinygltf::Accessor& posAcc=tModel->accessors[tPrimitive->attributes.at("POSITION")];
            const tinygltf::BufferView& posView=tModel->bufferViews[posAcc.bufferView];
            const float* positions=reinterpret_cast<const float*>(&tModel->buffers[posView.buffer].data[posView.byteOffset]);
            //Normals.
            bool hasNormals=hasAttribute(tPrimitive,"NORMAL");
            const float* normals=nullptr;
            if(hasNormals) {
                const tinygltf::Accessor* normAcc=&tModel->accessors[tPrimitive->attributes.at("NORMAL")];
                const tinygltf::BufferView* normView=&tModel->bufferViews[normAcc->bufferView];
                normals=reinterpret_cast<const float*>(&tModel->buffers[normView->buffer].data[normView->byteOffset]);
            }
            //UVs.
            bool hasUVs=hasAttribute(tPrimitive,"TEXCOORD_0");
            const float* uvs=nullptr;
            if(hasUVs) {
                const tinygltf::Accessor* uvAcc=&tModel->accessors[tPrimitive->attributes.at("TEXCOORD_0")];
                const tinygltf::BufferView* uvView=&tModel->bufferViews[uvAcc->bufferView];
                uvs=reinterpret_cast<const float*>(&tModel->buffers[uvView->buffer].data[uvView->byteOffset]);
            }
            //Tangents.
            bool hasTangents=hasAttribute(tPrimitive,"TANGENTS");
            const float* tangents=nullptr;
            if(hasTangents) {
                const tinygltf::Accessor* tanAcc=&tModel->accessors[tPrimitive->attributes.at("TANGENTS")];
                const tinygltf::BufferView* tanView=&tModel->bufferViews[tanAcc->bufferView];
                tangents=reinterpret_cast<const float*>(&tModel->buffers[tanView->buffer].data[tanView->byteOffset]);
            }
            //Bones (joints and weights).
            bool hasBones=hasAttribute(tPrimitive,"JOINTS_0")&&hasAttribute(tPrimitive,"WEIGHTS_0");
            const uint16_t* joints=nullptr;
            const float* weights=nullptr;
            if(hasBones) {
                const tinygltf::Accessor* jointAcc=&tModel->accessors[tPrimitive->attributes.at("JOINTS_0")];
                const tinygltf::Accessor* weightAcc=&tModel->accessors[tPrimitive->attributes.at("WEIGHTS_0")];
                const tinygltf::BufferView* jointView=&tModel->bufferViews[jointAcc->bufferView];
                const tinygltf::BufferView* weightView=&tModel->bufferViews[weightAcc->bufferView];
                joints=reinterpret_cast<const uint16_t*>(&tModel->buffers[jointView->buffer].data[jointView->byteOffset]);
                weights=reinterpret_cast<const float*>(&tModel->buffers[weightView->buffer].data[weightView->byteOffset]);
            }
            //Create vertexes.
            for(size_t i=0;i<posAcc.count;i++) {
                Vertex vert{};
                //Positions with rotation correction (because of different coordinate system).
                vert.position=glm::vec3(
                    positions[i*3+0],
                    positions[i*3+1],
                    positions[i*3+2]
                );
                //Normals.
                if(hasNormals)
                    vert.normal=glm::vec3(
                        normals[i*3+0],
                        normals[i*3+1],
                        normals[i*3+2]
                    );
                //Texture coords.
                if(hasUVs)
                    vert.uv=glm::vec2(
                        uvs[i*2+0],
                        uvs[i*2+1]
                    );
                //Tangents&bitangents.
                if(hasTangents) {
                    vert.tangent=glm::vec3(
                        tangents[i*4+0],
                        tangents[i*4+1],
                        tangents[i*4+2]
                    );
                    //const float handeness=(tangents[i*4+3]<0.f)?-1.f:1.f;
                    vert.bitangent=glm::cross(vert.normal,vert.tangent);
                } else if(uvs&&normals) {
                    //Index shifts.
                    size_t i1=(i+1)%posAcc.count;
                    size_t i2=(i+2)%posAcc.count;
                    //Get nearest positions and texture coords.
                    glm::vec3 pos1=glm::vec3(positions[i1*3+0],positions[i1*3+1],positions[i1*3+2]);
                    glm::vec3 pos2=glm::vec3(positions[i2*3+0],positions[i2*3+1],positions[i2*3+2]);
                    glm::vec2 uv1=glm::vec2(uvs[i1*2+0],uvs[i1*2+1]);
                    glm::vec2 uv2=glm::vec2(uvs[i2*2+0],uvs[i2*2+1]);
                    //Differences of positions and texture coords.
                    glm::vec3 deltaPos1=pos1-vert.position;
                    glm::vec3 deltaPos2=pos2-vert.position;
                    glm::vec2 deltaUV1=uv1-vert.uv;
                    glm::vec2 deltaUV2=uv2-vert.uv;
                    //Calculate difference.
                    float diff=1.f/(deltaUV1.x*deltaUV2.y-deltaUV1.y*deltaUV2.x);
                    //Get tangent and bitangent.
                    vert.tangent=glm::normalize(glm::vec3(
                        diff*(deltaUV2.y*deltaPos1.x-deltaUV1.y*deltaPos2.x),
                        diff*(deltaUV2.y*deltaPos1.y-deltaUV1.y*deltaPos2.y),
                        diff*(deltaUV2.y*deltaPos1.z-deltaUV1.y*deltaPos2.z)
                    ));
                    vert.tangent=glm::normalize(glm::vec3(
                        diff*(deltaUV1.x*deltaPos2.x-deltaUV2.x*deltaPos1.x),
                        diff*(deltaUV1.x*deltaPos2.y-deltaUV2.x*deltaPos1.y),
                        diff*(deltaUV1.x*deltaPos2.z-deltaUV2.x*deltaPos1.z)
                    ));
                }
                //Bones.
                if(hasBones)
                    for(size_t b=0;b<MAX_BONE_INFLUENCE;b++) {
                        vert.boneIDs[b]=joints?static_cast<int>(joints[i*4+b]):0;
                        vert.boneWeights[b]=weights?weights[i*4+b]:0.f;
                    }
                vertices.push_back(vert);
#ifdef FS_PRINT_DEBUG_MSGS
                tVert += 3;
                tNorm += 3;
                tTex += 2;
#endif
            }
            //Create indicies.
            const tinygltf::Accessor& indAcc=tModel->accessors[tPrimitive->indices];
            const tinygltf::BufferView& indView=tModel->bufferViews[indAcc.bufferView];
            const void* indData=&tModel->buffers[indView.buffer].data[indView.byteOffset];
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
                for(size_t i=0;i<indAcc.count;i++)
                    indices[i]=static_cast<unsigned int>(data32[i]);
                break;
            }
            default:
                LOG_ERRR("Unsupported index component type");
                break;
            }
            //Get material.
            Material* material=nullptr;
            if(tPrimitive->material>=0
                && tPrimitive->material<static_cast<int>(tModel->materials.size())
                && tPrimitive->material<static_cast<int>(tBaseModel->materials.size()))
                    material=&tBaseModel->materials[tPrimitive->material];

            return Mesh(vertices,indices,material);
        }

        Model load(std::string tPath, bool tBinary) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{"gltf model load"};
#endif // FS_INCLUDE_NVTX
            Model model{ tPath };

            tinygltf::Model gltf;
            tinygltf::TinyGLTF loader;
            std::string error, warn;

            bool res=tBinary?loader.LoadBinaryFromFile(&gltf,&error,&warn,tPath):loader.LoadASCIIFromFile(&gltf,&error,&warn,tPath);
            if(!error.empty())
                LOG_ERRR("Got error while loading model: " + error + "(at \"" + tPath + "\")");
            if(!warn.empty())
                LOG_ERRR("Got warning while loading model: " + warn + "(at \"" + tPath + "\")");
            if(!res) {
                LOG_ERRR("Failed to load model");
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Materials: %i",gltf.materials.size());
            LOGF_DBG("Meshes: %i",gltf.meshes.size());
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all materials.
            for(size_t m=0;m<gltf.materials.size();m++) {
                //Create material.
                const auto& mat = gltf.materials[m];
                Material material;
                //Load textures
                Texture diffuseTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.pbrMetallicRoughness.baseColorTexture.index), TT_DIFFUSE);
                Texture normalTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.normalTexture.index), TT_NORMAL);
                Texture specularTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.pbrMetallicRoughness.metallicRoughnessTexture.index), TT_SPECULAR);
                Texture ambientTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.occlusionTexture.index), TT_AMBIENT);
                Texture emissiveTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.emissiveTexture.index), TT_EMISSIVE);
                //Texture displacementTex=loadMaterialTexture(&model, mat.displacement_texname, "displacement");
                //Texture opacityTex=loadMaterialTexture(&model, mat.alpha_texname, "opacity");
                //Push back all textures.
                if(diffuseTex.ID!=0) material.textures.push_back(diffuseTex);
                if(normalTex.ID!=0) material.textures.push_back(normalTex);
                if(specularTex.ID!=0) material.textures.push_back(specularTex);
                if(ambientTex.ID!=0) material.textures.push_back(ambientTex);
                if(emissiveTex.ID!=0) material.textures.push_back(emissiveTex);
                //Get PBR data.
                material.params.emplace_back("diffuse",glm::vec3(mat.pbrMetallicRoughness.baseColorFactor[0],mat.pbrMetallicRoughness.baseColorFactor[1],mat.pbrMetallicRoughness.baseColorFactor[2]));
                material.params.emplace_back("emission",glm::vec3(mat.emissiveFactor[0],mat.emissiveFactor[1],mat.emissiveFactor[2]));
                material.params.emplace_back("specular",glm::vec3(0));
                material.params.emplace_back("ambientOcclusion",static_cast<float>(mat.occlusionTexture.strength));
                material.params.emplace_back("metallic",static_cast<float>(mat.pbrMetallicRoughness.metallicFactor));
                material.params.emplace_back("roughness",static_cast<float>(mat.pbrMetallicRoughness.roughnessFactor));
                model.materials.push_back(material);
            }
            //Process all meshes.
            for(size_t m=0;m<gltf.meshes.size();m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOGF_DBG("Processing mesh %i/%i",m+1,gltf.meshes.size());
#endif // FS_PRINT_DEBUG_MSGS
                for(size_t p=0;p<gltf.meshes[m].primitives.size();p++) {
#ifdef FS_PRINT_DEBUG_MSGS
                    LOGF_DBG("Processing premitive %i/%i",p+1,gltf.meshes[m].primitives.size());
                    model.meshes.push_back(processPrimitive(&model,&gltf,&gltf.meshes[m].primitives[p],model.getDirectory(),vert,ind,norm,tex));
#else
                    model.meshes.push_back(processPrimitive(&model,&gltf,&gltf.meshes[m].primitives[p],model.getDirectory()));
#endif // FS_PRINT_DEBUG_MSGS
                }
            }
            //Process all nodes.
            for(size_t c=0;c<gltf.scenes[gltf.defaultScene].nodes.size();c++)
                processNodes(&model,&gltf,gltf.scenes[gltf.defaultScene].nodes[c]);
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Nodes: %i",model.nodes.size());
            LOGF_DBG("Vertices: %i",vert/3);
            LOGF_DBG("Normals: %i",norm/3);
            LOGF_DBG("UVs: %i",tex/2);
#endif // FS_PRINT_DEBUG_MSGS
            return model;
        }
    }
}

#endif // !FS_LOADERS_GLTF_H
#endif // FS_LOADER_GLTF