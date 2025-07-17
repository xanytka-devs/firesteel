#ifndef FS_ENTITY_H
#define FS_ENTITY_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "common.hpp"
#include "utils/utils.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "utils/stbi_global.hpp"
#include "transform.hpp"
#include "model.hpp"
#ifdef FS_LOADER_OBJ
#include "loaders/obj.hpp"
#endif // FS_LOADER_OBJ
#ifdef FS_LOADER_GLTF
#include "loaders/gltf.hpp"
#endif // FS_LOADER_GLTF
#ifdef FS_LOADER_FBX
#ifdef FS_LOADER_OFBX
#include "loaders/fbx.hpp"
#else
#include "loaders/ufbx.hpp"
#endif // FS_LOADER_OFBX
#endif // FS_LOADER_FBX

namespace Firesteel {
    class Entity {
    public:
        Transform transform;
        Model model;

        Entity(const glm::vec3 tPos=glm::vec3(0), const glm::vec3 tRot=glm::vec3(0), const glm::vec3 tSize=glm::vec3(1)) {
            transform=Transform(tPos, tRot, tSize);
        }
        Entity(const std::string& tPath,
            const glm::vec3 tPos=glm::vec3(0), const glm::vec3 tRot=glm::vec3(0), const glm::vec3 tSize=glm::vec3(1)) {
            transform=Transform(tPos, tRot, tSize);
            load(tPath);
        }

        // Renders the model.
        void draw(const Shader* tShader) {
            if(!mHasMeshes) return;
            if(tShader==nullptr||!tShader->loaded) tShader=Shader::getDefaultShader();
            tShader->enable();
            tShader->setMat4("model", getMatrix());
            for(unsigned int i=0;i<model.meshes.size();i++)
                model.meshes[i].draw(tShader);
        }
        // Renders the model in the default shader.
        void draw() {draw(nullptr);}

        // Returns model matrix.
        glm::mat4 getMatrix() const {
            modelMatrix=glm::mat4(1);
            modelMatrix=glm::translate(modelMatrix, transform.position);
            modelMatrix=glm::rotate(modelMatrix, float(glm::radians(transform.rotation.x)), glm::vec3(1, 0, 0));
            modelMatrix=glm::rotate(modelMatrix, float(glm::radians(transform.rotation.y)), glm::vec3(0, 1, 0));
            modelMatrix=glm::rotate(modelMatrix, float(glm::radians(transform.rotation.z)), glm::vec3(0, 0, 1));
            modelMatrix=glm::scale(modelMatrix, transform.size);
            return modelMatrix;
        }

        bool hasModel() const { return mHasMeshes; }
        void remove() {
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed entity");
#endif // FS_PRINT_DEBUG_MSGS
            for(size_t i=0; i < model.meshes.size(); i++)
                model.meshes[i].remove();
            for(size_t i=0; i < model.materials.size(); i++)
                model.materials[i].remove();
            mHasMeshes=false;
        }
        void load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_WARN("Model at: \"" + tPath + "\" doesn't exist");
                return;
            }
            LOG_INFO("Loading model at: \"" + tPath + "\"");
            
            auto extBig=StrSplit(tPath, '.');
            std::string ext=extBig[extBig.size()-1];
            if(ext.empty()) {LOG_ERRR("Looks like the extension of given model is invalid");}
#ifdef FS_LOADER_OBJ
            else if(ext=="obj") model=OBJ::load(tPath);
#endif // FS_LOADER_OBJ
#ifdef FS_LOADER_GLTF
            else if(ext=="gltf"||ext=="glb") model=GLTF::load(tPath,ext=="glb");
#endif // FS_LOADER_GLTF
#ifdef FS_LOADER_FBX
            else if(ext=="fbx") model=FBX::load(tPath);
#endif // FS_LOADER_FBX
            else LOG_ERRR("Looks like \"" + ext + " \" model format isn't supported. Please try obj, gltf, glb or fbx.");

            LOG_INFO("Loaded model at: \"" + tPath + "\"");
            mHasMeshes=true;
        }

        void addMesh(const Mesh& tMesh) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices, %d indicies and %d textures\n",
                tMesh.vertices.size(), tMesh.indices.size(), tMesh.textures.size());
#endif // FS_PRINT_DEBUG_MSGS
            mHasMeshes=true;
            model.meshes.emplace_back(tMesh);
        }
        void addMesh(const std::vector<Vertex>& tVertices,
            const std::vector<unsigned int>& tIndices, const std::vector<Texture>& tTextures) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices, %d indicies and %d textures\n",
                tVertices.size(), tIndices.size(), tTextures.size());
#endif // FS_PRINT_DEBUG_MSGS
            mHasMeshes=true;
            model.meshes.emplace_back(tVertices,tIndices,tTextures);
        }

    private:
        static glm::mat4 modelMatrix;
        bool mHasMeshes=false;
    };
}

glm::mat4 Firesteel::Entity::modelMatrix=glm::mat4(1);

#endif // ! FS_ENTITY_H