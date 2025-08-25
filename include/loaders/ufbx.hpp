#ifndef FS_LOADER_OFBX
#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX

#include "../model.hpp"
#include "../utils/stbi_global.hpp"

#include "../../external/model_loaders/ufbx/ufbx.h"

namespace Firesteel {
    namespace FBX {
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        Texture loadMaterialTexture(Model* tModel, const std::string& texPath, const TextureType& type, size_t& tMatId) {
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
        Mesh processMesh(Model* tBaseModel, const ufbx_mesh* tMesh, size_t& tMatId
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices(tMesh->max_face_triangles*3);
            for(size_t f=0;f<tMesh->faces.count;f++) {
                ufbx_face face=tMesh->faces.data[f];
                //Triangulate the face.
                uint32_t numTri=ufbx_triangulate_face(
                    indices.data(), indices.size(), tMesh, face
                );
                //Loop through all triangle corners contiguosly.
                for(size_t i=0;i<numTri*3;i++) {
                    uint32_t index=indices[i];
                    Vertex vert{};

                    ufbx_vec3 v3=ufbx_get_vertex_vec3(&tMesh->vertex_position, index);
                    vert.position=glm::vec3(v3.x,v3.y,v3.z);                    
                    if(tMesh->vertex_normal.exists) {
                        v3=ufbx_get_vertex_vec3(&tMesh->vertex_normal, index);
                        vert.normal=glm::vec3(v3.x,v3.y,v3.z);
#ifdef FS_PRINT_DEBUG_MSGS
                        tNorm+=3;
#endif
                    }                    
                    if(tMesh->vertex_uv.exists) {
                        ufbx_vec2 v2=ufbx_get_vertex_vec2(&tMesh->vertex_uv, index);
                        vert.uv=glm::vec2(v2.x,v2.y);
#ifdef FS_PRINT_DEBUG_MSGS
                        tTex+=2;
#endif
                    }
                    if(tMesh->vertex_tangent.exists) {
                        v3=ufbx_get_vertex_vec3(&tMesh->vertex_tangent, index);
                        vert.tangent=glm::vec3(v3.x,v3.y,v3.z);
                    }
                    if(tMesh->vertex_bitangent.exists) {
                        v3=ufbx_get_vertex_vec3(&tMesh->vertex_bitangent, index);
                        vert.tangent=glm::vec3(v3.x,v3.y,v3.z);
                    } else if(tMesh->vertex_tangent.exists&&tMesh->vertex_normal.exists) vert.bitangent=glm::cross(vert.tangent,vert.normal);

                    vertices.push_back(vert);
                    indices.push_back(index);
#ifdef FS_PRINT_DEBUG_MSGS
                    tVert+=3;
                    tInd++;
#endif
                }
            }
            //Get textures.
            std::vector<Texture> textures;
            if(tMesh->materials.count>0) {
                const ufbx_material* mat=tMesh->materials.data[0];
                if(mat->fbx.diffuse_color.texture) textures.push_back(loadMaterialTexture(tBaseModel,mat->fbx.diffuse_color.texture->filename.data,TT_DIFFUSE, tMatId));
                if(mat->fbx.normal_map.texture) textures.push_back(loadMaterialTexture(tBaseModel,mat->fbx.normal_map.texture->filename.data,TT_NORMAL, tMatId));
                if(mat->fbx.specular_color.texture) textures.push_back(loadMaterialTexture(tBaseModel,mat->fbx.specular_color.texture->filename.data,TT_SPECULAR, tMatId));
                if(mat->fbx.emission_color.texture) textures.push_back(loadMaterialTexture(tBaseModel,mat->fbx.emission_color.texture->filename.data,TT_EMISSIVE, tMatId));
            }

            return Mesh(vertices,indices,textures);
        }

        Model load(const std::string& tPath) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{"fbx model load"};
#endif // FS_INCLUDE_NVTX
            Model model{ tPath };

            ufbx_error error{};
            ufbx_load_opts options{};
            options.ignore_missing_external_files=true;
            ufbx_scene* scene=ufbx_load_file(tPath.c_str(),&options,&error);
            //Check errors.
            if(error.type!=UFBX_ERROR_NONE) {
                LOG_ERRR("Got error while loading model (at "+tPath+"): "+error.info);
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Meshes: "+std::to_string(scene->meshes.count));
            LOG_DBG("Materials: "+std::to_string(scene->materials.count));
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            size_t matId=0;
            model.materials.resize(scene->materials.count);
            //Process all meshes.
            for(size_t m=0;m<scene->meshes.count;m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOGF_DBG("Processing mesh %i/%i",m+1,scene->meshes.count);
                model.meshes.push_back(processMesh(&model,scene->meshes[m],matId,vert,ind,norm,tex));
#else
                //model.meshes.push_back(processMesh(&model,scene->getMesh(static_cast<int>(m)),&materialIds));
#endif // FS_PRINT_DEBUG_MSGS
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

#endif // !FS_LOADERS_FBX
#endif // !FS_LOADER_OFBX