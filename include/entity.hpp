#ifndef FS_MODEL_H
#define FS_MODEL_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.hpp"
#include "utils/utils.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "utils/stbi_global.hpp"
#include "transform.hpp"

namespace Firesteel {

    struct Model {
        std::vector<Texture> textures;
        std::vector<Mesh> meshes;
        std::string path;
        std::string directory;
        Model(const std::string& tPath = "") {
            textures.clear();
            meshes.clear();
            path = tPath;
            directory = path.substr(0, path.find_last_of('\\'));
        }
    };

    class Entity {
    public:
        Transform transform;
        Model model;

        // Simplified constructor.
        Entity(glm::vec3 tPos = glm::vec3(0), glm::vec3 tRot = glm::vec3(0), glm::vec3 tSize = glm::vec3(1)) {
            transform = Transform(tPos, tRot, tSize);
        }

        // Constructor, expects a filepath to a 3D model.
        Entity(const std::string& tPath,
            glm::vec3 tPos = glm::vec3(0), glm::vec3 tRot = glm::vec3(0), glm::vec3 tSize = glm::vec3(1)) {
            transform = Transform(tPos, tRot, tSize);
            load(tPath);
        }

        // Renders the model.
        void draw(Shader* tShader) {
            if(!mHasMeshes) return;
            tShader->enable();
            tShader->setMat4("model", getMatrix());
            for(unsigned int i = 0; i < model.meshes.size(); i++)
                model.meshes[i].draw(tShader);
        }

        // Returns model matrix.
        glm::mat4 getMatrix() const {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, transform.position);
            modelMatrix = glm::rotate(modelMatrix, float(glm::radians(transform.rotation.x)), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, float(glm::radians(transform.rotation.y)), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, float(glm::radians(transform.rotation.z)), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, transform.size);
            return modelMatrix;
        }

        // Checks if entity has any meshes.
        bool hasModel() const { return mHasMeshes; }

        // Clears all meshes.
        void remove() {
            for(size_t i = 0; i < model.meshes.size(); i++)
                model.meshes[i].remove();
            for(size_t i = 0; i < model.textures.size(); i++)
                model.textures[i].remove();
            model.directory = "";
            mHasMeshes = false;
        }

        // Loads a model with ASSIMP.
        void load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_INFO("Model at: \"" + tPath + "\" doesn't exist");
                return;
            }
            //Read file via ASSIMP.
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(tPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            //Check for errors.
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                LOG_ERRR(std::string("Error while importing model: ") + importer.GetErrorString());
                return;
            }
            model = Model(tPath);

            LOG_INFO("Loading model at: \"" + tPath + "\"");
            processNode(scene->mRootNode, scene);
            LOG_INFO("Model has been successfully loaded");
            mHasMeshes = true;
        }

    private:
        static glm::mat4 modelMatrix;
        bool mHasMeshes = false;

        // Processes a node in a recursive fashion.
        void processNode(const aiNode* tNode, const aiScene* tScene) {
            //Process each mesh located at the current node.
            for (unsigned int i = 0; i < tNode->mNumMeshes; i++) {
                aiMesh* mesh = tScene->mMeshes[tNode->mMeshes[i]];
                model.meshes.push_back(processMesh(mesh, tScene));
            }
            // Process each of the children nodes.
            for (unsigned int i = 0; i < tNode->mNumChildren; i++)
                processNode(tNode->mChildren[i], tScene);
        }

        // Processes all vertex data to a Mesh.
        Mesh processMesh(const aiMesh* tMesh, const aiScene* tScene) {
            //Data to fill.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            //Walk through each of the mesh's vertices.
            for (unsigned int i = 0; i < tMesh->mNumVertices; i++) {
                Vertex vertex{};
                glm::vec3 vector{};
                //Positions.
                vector.x = tMesh->mVertices[i].x;
                vector.y = tMesh->mVertices[i].y;
                vector.z = tMesh->mVertices[i].z;
                vertex.position = vector;
                //Normals.
                if(tMesh->HasNormals()) {
                    vector.x = tMesh->mNormals[i].x;
                    vector.y = tMesh->mNormals[i].y;
                    vector.z = tMesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                //UVs.
                if(tMesh->mTextureCoords[0]) {
                    glm::vec2 vec{};
                    //UVs.
                    vec.x = tMesh->mTextureCoords[0][i].x;
                    vec.y = tMesh->mTextureCoords[0][i].y;
                    vertex.uv = vec;
                    //Tangent.
                    vector.x = tMesh->mTangents[i].x;
                    vector.y = tMesh->mTangents[i].y;
                    vector.z = tMesh->mTangents[i].z;
                    vertex.tangent = vector;
                    //Bitangent.
                    vector.x = tMesh->mBitangents[i].x;
                    vector.y = tMesh->mBitangents[i].y;
                    vector.z = tMesh->mBitangents[i].z;
                    vertex.bitangent = vector;
                } else vertex.uv = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }
            //Retrieve indicies.
            for(unsigned int i = 0; i < tMesh->mNumFaces; i++) {
                aiFace face = tMesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            //Process materials.
            aiMaterial* material = tScene->mMaterials[tMesh->mMaterialIndex];
            //Does the model even have textures?
            if(material->GetTextureCount(aiTextureType_DIFFUSE) == 0
                && material->GetTextureCount(aiTextureType_SPECULAR) == 0) {
                LOG_INFO("No general textures found. Using bound color data.");
                //Diffuse and specular color.
                aiColor4D def(1.0f);
                aiColor4D spec(1.0f);
                aiColor4D emis(1.0f);
                aiColor4D height(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &def);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emis);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &height);
                //Output.
                glm::vec4 def_v = glm::vec4(def.r, def.g, def.b, def.a);
                glm::vec4 spec_v = glm::vec4(spec.r, spec.g, spec.b, spec.a);
                glm::vec4 emis_v = glm::vec4(emis.r, emis.g, emis.b, emis.a);
                glm::vec4 height_v = glm::vec4(height.r, height.g, height.b, height.a);
                return Mesh(vertices, indices, def_v, spec_v, emis_v, height_v);
            }
            std::vector<Texture> texs;
            //Diffuse maps.
            texs = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();
            //Specular maps.
            texs = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();
            //Normal maps.
            texs = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();
            //Emission maps.
            texs = loadMaterialTextures(material, aiTextureType_EMISSIVE, "emission");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();
            //Height maps.
            texs = loadMaterialTextures(material, aiTextureType_AMBIENT, "height");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();
            //Opacity maps.
            texs = loadMaterialTextures(material, aiTextureType_OPACITY, "opacity");
            textures.insert(textures.end(), texs.begin(), texs.end());
            texs.clear();

            if(textures.size()==0) LOG_WARN("Model somehow didn't load any textures");

            return Mesh(vertices, indices, textures);
        }

        // Checks all material textures of a given type and loads the textures if they're not loaded yet.
        std::vector<Texture> loadMaterialTextures(const aiMaterial* tMat, const aiTextureType& tType, const std::string& tTypeName) {
            std::vector<Texture> textures;
            for(unsigned int i = 0; i < tMat->GetTextureCount(tType); i++) {
                aiString str;
                tMat->GetTexture(tType, i, &str);
                //Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for(unsigned int j = 0; j < model.textures.size(); j++) {
                    if(std::strcmp(model.textures[j].path.data(), str.C_Str()) != 0) continue;
                    //Optimization to disable multiple loads for texture.
                    textures.push_back(model.textures[j]);
                    skip = true;
                    break;
                }
                if(skip) continue;
                //If texture hasn't been loaded already, load it.
                Texture texture;
                texture.ID = TextureFromFile(this->model.directory + "/" + str.C_Str(), &texture.isMonochrome, true);
                texture.type = tTypeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                model.textures.push_back(texture);
            }
            return textures;
        }
    };

}

glm::mat4 Firesteel::Entity::modelMatrix = glm::mat4(1);

#endif // ! FS_MODEL_H