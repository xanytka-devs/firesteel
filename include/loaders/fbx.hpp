#ifndef FS_LOADER_OFBX
#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX

#include "../mesh.hpp"
#include "../utils/stbi_global.hpp"

#include "../../external/model_loaders/ufbx/ufbx.h"

namespace Firesteel {
    namespace FBX {
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        Texture loadMaterialTexture(const Model* tModel, const char* tTexPath, const TextureType& tType) {
            if(!tTexPath) return {};
            std::string texPath(tTexPath);
            if(texPath.empty()) return {};
            //Get full path.
            std::string fullPath=texPath;
            //Check all materials for this texture.
            for (auto& material : tModel->materials) {
                for (const auto& texture : material.textures) {
                    if (texture.path == texPath && texture.type == tType) {
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
            texture.type=tType;
            texture.path=texPath;
            texture.ID=TextureFromFile(fullPath.c_str(), &texture.isMonochrome, true);

            return texture;
        }
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        Mesh processMesh(Model* tBaseModel, const ufbx_mesh* tMesh
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
                        vert.uv=glm::vec2(v2.x,1.f-v2.y);
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
            //Get material.
            Material* material=nullptr;
            if(tMesh->materials.count>0
                && tMesh->materials.data[0]->typed_id<static_cast<uint32_t>(tBaseModel->materials.size()))
                material=&tBaseModel->materials[tMesh->materials[0]->typed_id];

            return Mesh(vertices,indices,material);
        }
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        void processNodes(Model* tBaseModel, const ufbx_scene* tModel, const ufbx_node* tNode, Node* tParent=nullptr) {
            Node node;
            node.name=std::string(tNode->name.data,tNode->name.length);
            if(node.name.empty()) node.name="Node_"+std::to_string(tNode->typed_id);
            //Apply translations to node.
            const ufbx_transform& t=tNode->local_transform;
            node.transform.position=glm::vec3(t.translation.x,t.translation.y,t.translation.z);
            node.transform.rotation=glm::degrees(glm::eulerAngles(
                glm::quat(CASTF(t.rotation.w),CASTF(t.rotation.x),CASTF(t.rotation.y),CASTF(t.rotation.z)
            )));
            node.transform.size=glm::vec3(t.scale.x,t.scale.y,t.scale.z);
            if(tNode->mesh)
                for (size_t i=0;i<tModel->meshes.count;i++)
                    if(tModel->meshes.data[i]==tNode->mesh) {
                        node.index=static_cast<int>(i);
                        break;
                    }
            if(tParent) tParent->children.push_back(node);
            else tBaseModel->nodes.push_back(node);
            for(size_t c=0;c<tNode->children.count;c++)
                processNodes(tBaseModel,tModel,tNode->children.data[c],(tParent?&tParent->children.back():&tBaseModel->nodes.back()));
        }
        /// [!WARING]
        /// This function is internal and only used for the FBX loader. Use it at your own risk.
        void tryAddTexture(const ufbx_material_map* tTex, Model* tModel, Material* tMat, const TextureType& tType) {
            if(!tTex||!tModel||!tMat) return;
            if(!tTex->has_value) return;
            if(!tTex->texture) return;
            Texture tex = loadMaterialTexture(tModel, tTex->texture->filename.data, tType);
            if(tex.ID != 0) tMat->textures.push_back(tex);
        }

        Model load(const std::string& tPath) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{"fbx model load"};
#endif // FS_INCLUDE_NVTX
            Model model{ tPath };

            ufbx_error error{};
            ufbx_load_opts options{};
            options.ignore_missing_external_files=true;
            options.target_unit_meters=1;
            options.space_conversion=UFBX_SPACE_CONVERSION_ADJUST_TRANSFORMS;
            ufbx_scene* scene=ufbx_load_file(tPath.c_str(),&options,&error);
            //Check errors.
            if(error.type!=UFBX_ERROR_NONE) {
                LOG_ERRR("Got error while loading model (at "+tPath+"): "+error.info);
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Meshes: %i",scene->meshes.count);
            LOGF_DBG("Materials: %i",scene->materials.count);
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all materials.
            for(size_t m=0;m<scene->materials.count;m++) {
                //Create material.
                const auto& mat=scene->materials.data[m];
                Material material;
                //Load textures.
                tryAddTexture(&mat->pbr.base_color,&model,&material,TT_DIFFUSE);
                tryAddTexture(&mat->pbr.normal_map,&model,&material,TT_NORMAL);
                tryAddTexture(&mat->pbr.specular_color,&model,&material,TT_SPECULAR);
                tryAddTexture(&mat->pbr.ambient_occlusion,&model,&material,TT_AMBIENT);
                tryAddTexture(&mat->pbr.displacement_map,&model,&material,TT_DISPLACEMENT);
                tryAddTexture(&mat->pbr.opacity,&model,&material,TT_OPACITY);
                tryAddTexture(&mat->pbr.emission_color,&model,&material,TT_EMISSIVE);
                //Get PBR data.
                material.params.emplace_back("diffuse",glm::vec3(mat->pbr.base_color.value_vec3.x,mat->pbr.base_color.value_vec3.y,mat->pbr.base_color.value_vec3.z));
                material.params.emplace_back("emission",glm::vec3(mat->pbr.emission_color.value_vec3.x,mat->pbr.emission_color.value_vec3.y,mat->pbr.emission_color.value_vec3.z));
                material.params.emplace_back("specular",glm::vec3(mat->pbr.specular_color.value_vec3.x,mat->pbr.specular_color.value_vec3.y,mat->pbr.specular_color.value_vec3.z));
                material.params.emplace_back("ambientOcclusion",static_cast<float>(mat->pbr.ambient_occlusion.value_real));
                material.params.emplace_back("metallic",static_cast<float>(mat->pbr.metalness.value_real));
                material.params.emplace_back("roughness",static_cast<float>(mat->pbr.roughness.value_real));
                model.materials.push_back(material);
            }
            //Process all meshes.
            for(size_t m=0;m<scene->meshes.count;m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOGF_DBG("Processing mesh %i/%i",m+1,scene->meshes.count);
                model.meshes.push_back(processMesh(&model,scene->meshes[m],vert,ind,norm,tex));
#else
                model.meshes.push_back(processMesh(&model,scene->meshes[m]));
#endif // FS_PRINT_DEBUG_MSGS
            }
            //Process all nodes.
            for(size_t c=0;c<scene->root_node->children.count;c++)
                processNodes(&model,scene,scene->root_node->children.data[c]);
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

#endif // !FS_LOADERS_FBX
#endif // !FS_LOADER_OFBX