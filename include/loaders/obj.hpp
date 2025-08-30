#ifdef FS_LOADER_OBJ
#ifndef FS_LOADERS_OBJ_H
#define FS_LOADERS_OBJ_H

#include "../mesh.hpp"
#include "../utils/stbi_global.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../external/model_loaders/tiny_obj_loader.h"

namespace Firesteel {
    namespace OBJ {
        /// [!WARING]
        /// This function is internal and only used for the OBJ loader. Use it at your own risk.
        Texture loadMaterialTexture(const Model* tModel, const std::string& texPath, const TextureType& type) {
            if(texPath.empty()) return {};
            //Get full path.
            std::string fullPath=tModel->getDirectory() + "/" + texPath;
            //Check all materials for this texture.
            for(size_t m=0;m<tModel->materials.size();m++) {
                auto& material = tModel->materials[m];
                for(size_t t=0;t<material.textures.size();t++) {
                    const auto& texture = material.textures[t];
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
        Mesh processShape(Model* tModel, const tinyobj::shape_t& tShape, const tinyobj::attrib_t& tAttrib, int materialID, std::vector<std::string>& nodeNames
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Indicies map.
            std::unordered_map<std::string, unsigned int> indexMap;
            unsigned int index=0;
            //Load de model.
            for (size_t i=0; i < tShape.mesh.num_face_vertices.size(); ++i) {
                unsigned int num_face_verts=tShape.mesh.num_face_vertices[i];
                for(unsigned int j=0; j < num_face_verts; ++j) {
                    tinyobj::index_t idx=tShape.mesh.indices[i * num_face_verts + j];
                    //Get valid key.
                    std::ostringstream keyStream;
                    keyStream << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index;
                    std::string key=keyStream.str();

                    if(indexMap.find(key) == indexMap.end()) {
                        Vertex vertex{};
                        //Position.
                        vertex.position={
                            tAttrib.vertices[3*idx.vertex_index+0],
                            tAttrib.vertices[3*idx.vertex_index+1],
                            tAttrib.vertices[3*idx.vertex_index+2]
                        };
                        //Normal.
                        if(idx.normal_index >= 0) {
                            vertex.normal={
                                tAttrib.normals[3*idx.normal_index+0],
                                tAttrib.normals[3*idx.normal_index+1],
                                tAttrib.normals[3*idx.normal_index+2]
                            };
#ifdef FS_PRINT_DEBUG_MSGS
                            tNorm += 1;
#endif
                        }
                        //UV.
                        if(idx.texcoord_index >= 0) {
                            vertex.uv={
                                tAttrib.texcoords[2*idx.texcoord_index+0],
                                1.f - tAttrib.texcoords[2 * idx.texcoord_index+1]
                            };
#ifdef FS_PRINT_DEBUG_MSGS
                            tTex += 1;
#endif
                        }
                        //Tangent & bitangent (obj doesn't support those, so just set them to zero).
                        vertex.tangent=glm::vec3(0.0f);
                        vertex.bitangent=glm::vec3(0.0f);
                        //Bones & weights (obj doesn't support those, so just set them to zero).
                        for (int k=0; k < MAX_BONE_INFLUENCE; ++k) {
                            vertex.boneIDs[k]=0;
                            vertex.boneWeights[k]=0.0f;
                        }
                        vertices.push_back(vertex);
#ifdef FS_PRINT_DEBUG_MSGS
                        tVert += 1;
#endif
                        indexMap[key]=index++;
                    }
                    indices.push_back(indexMap[key]);
#ifdef FS_PRINT_DEBUG_MSGS
                    tInd += 1;
#endif
                }
            }
            //Get material.
            Material* material=nullptr;
            if(materialID>=0 && materialID<static_cast<int>(tModel->materials.size()))
                material=&tModel->materials[materialID];

            nodeNames.push_back(tShape.name);
            return Mesh(vertices, indices, material);
        }

        Model load(std::string tPath) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{"obj model load"};
#endif // FS_INCLUDE_NVTX
            Model model{ tPath };

            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path=""; //Path to material files.
            tinyobj::ObjReader reader;
            //Load model from given file.
            if(!reader.ParseFromFile(tPath, reader_config)) {
                if(!reader.Error().empty())
                    LOG_ERRR("Got error while loading model: " + reader.Error() + "(at \"" + tPath + "\")");
                return model;
            }
            //Report errors (if found).
            if(!reader.Warning().empty())
                LOG_WARN("Got warning while loading model: " + reader.Warning() + "(at \"" + tPath + "\")");
            //Initialize variables.
            auto& attrib=reader.GetAttrib();
            auto& shapes=reader.GetShapes();
            auto& materials=reader.GetMaterials();
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Materials: %i",materials.size());
            LOGF_DBG("Meshes: %i",shapes.size());
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all materials.
            for (size_t m=0;m<materials.size();m++) {
                //Create material.
                const auto& mat = materials[m];
                Material material;
                //Load textures.
                Texture diffuseTex=loadMaterialTexture(&model, mat.diffuse_texname, TT_DIFFUSE);
                Texture normalTex=loadMaterialTexture(&model, mat.normal_texname, TT_NORMAL);
                Texture specularTex=loadMaterialTexture(&model, mat.specular_texname, TT_SPECULAR);
                Texture ambientTex=loadMaterialTexture(&model, mat.ambient_texname, TT_AMBIENT);
                Texture displacementTex=loadMaterialTexture(&model, mat.displacement_texname, TT_DISPLACEMENT);
                Texture opacityTex=loadMaterialTexture(&model, mat.alpha_texname, TT_OPACITY);
                Texture emissiveTex=loadMaterialTexture(&model, mat.emissive_texname, TT_EMISSIVE);
                //Push back all textures.
                if(diffuseTex.ID!=0) material.textures.push_back(diffuseTex);
                if(normalTex.ID!=0) material.textures.push_back(normalTex);
                if(specularTex.ID!=0) material.textures.push_back(specularTex);
                if(ambientTex.ID!=0) material.textures.push_back(ambientTex);
                if(displacementTex.ID!=0) material.textures.push_back(displacementTex);
                if(opacityTex.ID!=0) material.textures.push_back(opacityTex);
                if(emissiveTex.ID!=0) material.textures.push_back(emissiveTex);
                //Get PBR data.
                material.params.emplace_back("albedo",glm::vec3(mat.diffuse[0],mat.diffuse[1],mat.diffuse[2]));
                material.params.emplace_back("emission",glm::vec3(mat.emission[0],mat.emission[1],mat.emission[2]));
                material.params.emplace_back("specular",glm::vec3(mat.specular[0],mat.specular[1],mat.specular[2]));
                material.params.emplace_back("ambientOcclusion",1);
                material.params.emplace_back("metallic",(float)mat.metallic);
                material.params.emplace_back("roughness",(float)mat.roughness);
                model.materials.push_back(material);
            }
            //Process all shapes.
            std::vector<std::string> nodeNames;
            for(size_t s=0;s<shapes.size();s++) {
                const auto& shape=shapes[s];
                int materialID=shape.mesh.material_ids[0];
#ifdef FS_PRINT_DEBUG_MSGS
                LOGF_DBG("Processing mesh %i/%i",s+1,shapes.size());
                model.meshes.push_back(processShape(&model, shape, attrib, materialID, nodeNames, vert, ind, norm, tex));
#else
                model.meshes.push_back(processShape(&model, shape, attrib, materialID, nodeNames));
#endif // FS_PRINT_DEBUG_MSGS
            }
            //Process all nodes.
            Node root;
            root.name="[Root]";
            for(size_t n=0;n<model.meshes.size();n++) {
                Node mesh;
                mesh.index=static_cast<int>(n);
                mesh.name=nodeNames[n];
                root.children.push_back(mesh);
            }
            model.nodes.push_back(root);
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

#endif // !FS_LOADERS_OBJ_H
#endif // FS_LOADER_OBJ