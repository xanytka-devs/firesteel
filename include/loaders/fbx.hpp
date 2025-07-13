#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX
#ifdef FS_LOADER_FBX

#include "../model.hpp"
#include "../utils/stbi_global.hpp"

#define OFBX_USE_CPP_ALLOCATOR
#include "../../external/model_loaders/ofbx/ofbx.h"

namespace Firesteel {
    namespace FBX {
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        std::vector<uint8_t> readFileBinary(const std::string& tPath) {
            //Try to get file stream.
            std::ifstream file(tPath,std::ios::binary|std::ios::ate);
            if(!file.is_open()) {
                LOG_ERRR("Failed to open model file (at " + tPath + ")");
                return {};
            }
            //Read the file.
            size_t size=file.tellg();
            std::vector<uint8_t> buffer(size);
            file.seekg(0);
            file.read((char*)buffer.data(),size);
            file.close();
            return buffer;
        }

        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        Texture loadMaterialTexture(Model* tModel, const std::string& texPath, const std::string& type, const size_t& tMatId) {
            if(texPath.empty()) return {};
            //Get full path.
            std::string fullPath=texPath;
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
            tModel->materials[tMatId].textures.emplace_back(texture);

            return texture;
        }

        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        Mesh processMesh(Model* tModel, const ofbx::Mesh* tMesh, std::vector<ofbx::u64>* tMatIds
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Initialize getters.
            const ofbx::GeometryData& geometry=tMesh->getGeometryData();
            const int vertexCount=geometry.getPositions().values_count;
            const ofbx::Vec3* positions=geometry.getPositions().values;
            const int* indicies=geometry.getPositions().indices;
            const ofbx::Vec3* normals=geometry.getNormals().values;
            const ofbx::Vec3* tangents=geometry.getTangents().values;
            const int* uvInds=geometry.getUVs().indices;
            const ofbx::Vec2* uvs=geometry.getUVs().values;
            //Create vertexes.
            for(size_t i=0;i<vertexCount;i++) {
                Vertex vert{};
                //Position.
                vert.position=glm::vec3(
                    positions[indicies[i]].x,
                    positions[indicies[i]].y,
                    positions[indicies[i]].z
                );
                //Normal.
                if(normals)
                    vert.normal=glm::vec3(
                        normals[i].x,
                        normals[i].y,
                        normals[i].z
                    );
                //Texture coords.
                if(uvs)
                    vert.uv=glm::vec2(
                        uvs[uvInds[i]].x,
                        1.f-uvs[uvInds[i]].y
                    );
                //Tangents.
                if(tangents)
                    vert.tangent=glm::vec3(
                        tangents[i].x,
                        tangents[i].y,
                        tangents[i].z
                    );
                //Bitangents.
                if(tangents&&normals) vert.bitangent=glm::cross(vert.tangent,vert.normal);
                //Bones.
                /*if(tMesh->getSkin().count>0) {
                    const ufbx_skin_deformer* skin=tMesh->skin_deformers.data[0];
                    for (size_t b=0;b<MAX_BONE_INFLUENCE;b++) {
                        vert.boneIDs[b]=skin->weights[b].cluster_index;
                        vert.boneWeights[b]=static_cast<float>(skin->weights[b].weight);
                    }
                }*/
#ifdef FS_PRINT_DEBUG_MSGS
                tVert+=3;
                tNorm+=3;
                tTex+=2;
#endif
                //Texture coords.
                vertices.push_back(vert);
            }
            //Create indicies.
            for(unsigned int i=0;i<static_cast<unsigned int>(tMesh->getGeometryData().getPositions().count);i++) {
                indices.push_back(i);
#ifdef FS_PRINT_DEBUG_MSGS
                tInd+=1;
#endif
            }

            //Get material.
            std::vector<Texture> textures;
            auto mat=tMesh->getMaterial(0);
            size_t mId=0;
            for(size_t j=0;j<tMatIds->size();j++)
                if(tMatIds->at(j) == mat->id) {
                    mId=j;
                    break;
                } else mId=tMatIds->size();
            if(mId==tMatIds->size()) {
                tMatIds->push_back(mat->id);
                tModel->materials.push_back({{}});
                //Get textures.
                char filename[127];
                if(mat->getTexture(ofbx::Texture::DIFFUSE)) {
                    mat->getTexture(ofbx::Texture::DIFFUSE)->getFileName().toString(filename);
                    textures.emplace_back(loadMaterialTexture(tModel, filename, "diffuse", mId));
                }
                if(mat->getTexture(ofbx::Texture::NORMAL)) {
                    mat->getTexture(ofbx::Texture::NORMAL)->getFileName().toString(filename);
                    textures.emplace_back(loadMaterialTexture(tModel, filename, "normal", mId));
                }
                if(mat->getTexture(ofbx::Texture::SPECULAR)) {
                    mat->getTexture(ofbx::Texture::SPECULAR)->getFileName().toString(filename);
                    textures.emplace_back(loadMaterialTexture(tModel, filename, "specular", mId));
                }
                if(mat->getTexture(ofbx::Texture::AMBIENT)) {
                    mat->getTexture(ofbx::Texture::AMBIENT)->getFileName().toString(filename);
                    textures.emplace_back(loadMaterialTexture(tModel, filename, "ambient", mId));
                }
                if(mat->getTexture(ofbx::Texture::EMISSIVE)) {
                    mat->getTexture(ofbx::Texture::EMISSIVE)->getFileName().toString(filename);
                    textures.emplace_back(loadMaterialTexture(tModel, filename, "emissive", mId));
                }
                //Texture displacementTex=loadMaterialTexture(&model, mat.displacement_texname, "displacement");
                //Texture opacityTex=loadMaterialTexture(&model, mat.alpha_texname, "opacity");
                //Check all textures.
                for(size_t t=0;t<textures.size();t++) {
                    //Texture is valid.
                    if(textures[t].ID!=0) continue;
                    //Texture either doesn't exist or is corrupted.
                    textures.erase(textures.begin()+t);
                    t-=1;
                }
            } else {
#ifdef FS_PRINT_DEBUG_MSGS
                LOG_DBG("Given material's textures are already loaded");
#endif
                textures=tModel->materials[mId].textures;
            }

            return Mesh(vertices,indices,textures);
        }

        Model load(std::string tPath) {
            Model model{ tPath };

            //Get file contents.
            auto data=readFileBinary(tPath);
            if(data.empty()) {
                LOG_ERRR("Got empty file while loading model (at "+tPath+")");
                return model;
            }
            auto flags =
                ofbx::LoadFlags::IGNORE_BLEND_SHAPES
                | ofbx::LoadFlags::IGNORE_CAMERAS
                | ofbx::LoadFlags::IGNORE_LIGHTS
                //| ofbx::LoadFlags::IGNORE_TEXTURES
                //| ofbx::LoadFlags::IGNORE_SKIN
                //| ofbx::LoadFlags::IGNORE_BONES
                | ofbx::LoadFlags::IGNORE_PIVOTS
                | ofbx::LoadFlags::IGNORE_ANIMATIONS
                //| ofbx::LoadFlags::IGNORE_MATERIALS
                | ofbx::LoadFlags::IGNORE_POSES
                | ofbx::LoadFlags::IGNORE_VIDEOS
                | ofbx::LoadFlags::IGNORE_LIMBS
                //| ofbx::LoadFlags::IGNORE_MESHES
                //| ofbx::LoadFlags::IGNORE_MODELS
                ;
            const ofbx::IScene* scene=ofbx::load(data.data(),data.size(),(ofbx::u16)flags);
            if(!scene) {
                LOG_ERRR("Failed to load model scene");
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Meshes: "+std::to_string(scene->getGeometryCount()));
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all meshes.
            std::vector<ofbx::u64> materialIds;
            for(size_t m=0;m<scene->getMeshCount();m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOG_DBG("Processing mesh "+std::to_string((int)(m+1))+"/"+std::to_string((int)scene->getGeometryCount()));
                model.meshes.push_back(processMesh(&model,scene->getMesh(static_cast<int>(m)),&materialIds,vert,ind,norm,tex));
#else
                model.meshes.push_back(processMesh(&model,scene->getMesh(static_cast<int>(m)),&materialIds));
#endif // FS_PRINT_DEBUG_MSGS
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Materials: "+std::to_string(model.materials.size()));
            LOG_DBG("Vertices: "+std::to_string((int)(vert) / 3));
            LOG_DBG("Normals: "+std::to_string((int)(norm) / 3));
            LOG_DBG("UVs: "+std::to_string((int)(tex) / 2));
#endif // FS_PRINT_DEBUG_MSGS
            //Cleanup.
            data.clear();
            materialIds.clear();
            return model;
        }
    }
}

#endif // FS_LOADER_FBX
#endif // !FS_LOADERS_FBX