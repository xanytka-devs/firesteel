#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX
#ifdef FS_LOADER_FBX

#include "../model.hpp"
#include "../utils/stbi_global.hpp"

#define UFBX_IMPL
#include "../../external/ufbx/ufbx.h"

namespace Firesteel {
    namespace FBX {
        /// [!WARING]
        /// This function is internal and only used for the OBJ loader. Use it at your own risk.
        Texture loadMaterialTexture(const Model* tModel, const std::string& texPath, const std::string& type) {
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

            return texture;
        }

        /// [!WARING]
        /// This function is internal and only used for the OBJ loader. Use it at your own risk.
        Mesh processMesh(Model* tModel, ufbx_mesh* tMesh
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Create vertexes.
            for(size_t i=0;i<tMesh->num_vertices;i++) {
                Vertex vert{};
                //Position.
                vert.position=glm::vec3(
                    tMesh->vertex_position[i].x,
                    tMesh->vertex_position[i].y,
                    tMesh->vertex_position[i].z
                );
                //Normal.
                if(tMesh->vertex_normal.values.data)
                    vert.normal=glm::vec3(
                        tMesh->vertex_normal[i].x,
                        tMesh->vertex_normal[i].y,
                        tMesh->vertex_normal[i].z
                    );
                //Texture coords.
                if(tMesh->uv_sets.data) {
                    auto uvVec=tMesh->vertex_uv[i];
                    vert.uv=glm::vec2(
                        uvVec.x,
                        1.f-uvVec.y
                    );
                }
                //Tangents.
                if(tMesh->vertex_tangent.exists)
                    vert.tangent=glm::vec3(
                        tMesh->vertex_tangent[i].x,
                        tMesh->vertex_tangent[i].y,
                        tMesh->vertex_tangent[i].z
                    );
                //Bitangents.
                if(tMesh->vertex_bitangent.exists)
                    vert.bitangent=glm::vec3(
                        tMesh->vertex_bitangent[i].x,
                        tMesh->vertex_bitangent[i].y,
                        tMesh->vertex_bitangent[i].z
                    );
                //Bones.
                if(tMesh->skin_deformers.count>0) {
                    const ufbx_skin_deformer* skin=tMesh->skin_deformers.data[0];
                    for (size_t b=0;b<MAX_BONE_INFLUENCE;b++) {
                        LOG("Bone: "+std::to_string(b));
                        vert.boneIDs[b]=skin->weights[b].cluster_index;
                        vert.boneWeights[b]=static_cast<float>(skin->weights[b].weight);
                    }
                }
#ifdef FS_PRINT_DEBUG_MSGS
                tVert+=3;
                tNorm+=3;
                tTex+=2;
#endif
                //Texture coords.
                vertices.push_back(vert);
            }
            //Create indicies.
            for(size_t i=0;i<tMesh->num_indices;i++) {
                indices.push_back(tMesh->vertex_indices.data[i]);
#ifdef FS_PRINT_DEBUG_MSGS
                tInd+=1;
#endif
            }

            //Get textures.
            Texture diffuseTex=loadMaterialTexture(tModel, tMesh->materials[0]->textures[0].texture->filename.data, "diffuse");
            //Texture normalTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.normalTexture.index), "normal");
            //Texture specularTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.pbrMetallicRoughness.metallicRoughnessTexture.index), "specular");
            //Texture ambientTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.occlusionTexture.index), "ambient");
            //Texture displacementTex=loadMaterialTexture(&model, mat.displacement_texname, "displacement");
            //Texture opacityTex=loadMaterialTexture(&model, mat.alpha_texname, "opacity");
            //Push back all textures.
            std::vector<Texture> textures;
            if(diffuseTex.ID!=0) textures.push_back(diffuseTex);
            //if(normalTex.ID!=0) textures.push_back(normalTex);
            //if(specularTex.ID!=0) textures.push_back(specularTex);
            //if(ambientTex.ID!=0) textures.push_back(ambientTex);
            //if(displacementTex.ID!=0) textures.push_back(displacementTex);
            //if(opacityTex.ID!=0) textures.push_back(opacityTex);

            return Mesh(vertices,indices,textures);
        }

        Model load(std::string tPath) {
            Model model{ tPath };
            
            ufbx_load_opts opts={};
            ufbx_error error;
            ufbx_scene *scene=ufbx_load_file(tPath.c_str(), &opts, &error);
            if(!scene) {
                LOG_ERRR("Got error while loading model: " + std::string(error.description.data) + "(at \"" + tPath + "\")");
                LOG_ERRR("Failed to load model");
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Materials: "+std::to_string((int)scene->materials.count));
            LOG_DBG("Meshes: "+std::to_string((int)scene->meshes.count));
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all materials (texture).
            for(const auto& mat : scene->materials) {
                Texture diffuseTex=loadMaterialTexture(&model, mat->textures[0].texture->filename.data, "diffuse");
                //Texture normalTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.normalTexture.index), "normal");
                //Texture specularTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.pbrMetallicRoughness.metallicRoughnessTexture.index), "specular");
                //Texture ambientTex=loadMaterialTexture(&model, getTexPath(&gltf,mat.occlusionTexture.index), "ambient");
                //Texture displacementTex=loadMaterialTexture(&model, mat.displacement_texname, "displacement");
                //Texture opacityTex=loadMaterialTexture(&model, mat.alpha_texname, "opacity");
                //Push back all textures.
                std::vector<Texture> textures;
                if(diffuseTex.ID!=0) textures.push_back(diffuseTex);
                //if(normalTex.ID!=0) textures.push_back(normalTex);
                //if(specularTex.ID!=0) textures.push_back(specularTex);
                //if(ambientTex.ID!=0) textures.push_back(ambientTex);
                //if(displacementTex.ID!=0) textures.push_back(displacementTex);
                //if(opacityTex.ID!=0) textures.push_back(opacityTex);
                model.materials.push_back({ textures });
            }
            //Process all meshes.
            for(size_t m=0;m<scene->meshes.count;m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOG_DBG("Processing mesh "+std::to_string((int)(m+1))+"/"+std::to_string((int)scene->meshes.count));
                model.meshes.push_back(processMesh(&model,scene->meshes.data[m],vert,ind,norm,tex));
#else
                model.meshes.push_back(processMesh(&model,scene->meshes.data[m]));
#endif // FS_PRINT_DEBUG_MSGS
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Vertices: "+std::to_string((int)(vert) / 3));
            LOG_DBG("Normals: "+std::to_string((int)(norm) / 3));
            LOG_DBG("UVs: "+std::to_string((int)(tex) / 2));
#endif // FS_PRINT_DEBUG_MSGS
            ufbx_free_scene(scene);
            return model;
        }
    }
}

#endif // FS_LOADER_FBX
#endif // !FS_LOADERS_FBX