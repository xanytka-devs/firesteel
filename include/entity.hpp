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
#ifdef FS_LOADER_OBJ
#include "loaders/obj.hpp"
#endif // FS_LOADER_OBJ
#ifdef FS_LOADER_GLTF
#include "loaders/gltf.hpp"
#endif // FS_LOADER_GLTF
#ifdef FS_LOADER_FBX
#include "loaders/fbx.hpp"
#endif // FS_LOADER_FBX

namespace Firesteel {
    class Entity {
    public:
        Entity(const glm::vec3 tPos=glm::vec3(0), const glm::vec3 tRot=glm::vec3(0), const glm::vec3 tSize=glm::vec3(1))
            : transform(Transform(tPos, tRot, tSize)) { }
        Entity(const std::string& tPath,
            const glm::vec3 tPos=glm::vec3(0), const glm::vec3 tRot=glm::vec3(0), const glm::vec3 tSize=glm::vec3(1))
                : transform(Transform(tPos, tRot, tSize)) {
            load(tPath);
        }

        // Renders the model with given material or default shader.
        void draw() {
            if(model.meshes.size()==0) return;
            for(size_t n=0;n<model.nodes.size();n++)
                drawNode(model.nodes[n], transform.getMatrix());
        }
        // Replaces materials with default shader with given shader.
        void setMaterialsShader(std::shared_ptr<Shader> tShader, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(size_t m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->ID==Shader::getDefaultShader()->ID||tReplaceAll) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces materials with shader id with given shader.
        void replaceMaterialsShader(const unsigned int& tIdToReplace, std::shared_ptr<Shader> tShader) {
            if(!hasModel()) return;
            for(size_t m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->ID==tIdToReplace) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Replaced entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces all materials with given one.
        void setMaterial(Material* tMaterial, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(size_t m = 0; m < model.materials.size(); m++)
                if(model.materials[m].getShader()->ID==Shader::getDefaultShader()->ID||tReplaceAll) model.materials[m]=*tMaterial;
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity material");
#endif // FS_PRINT_DEBUG_MSGS
        }

        bool hasModel() const { return model.meshes.size()!=0; }
        void remove() {
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed entity");
#endif // FS_PRINT_DEBUG_MSGS
            for(size_t i=0;i<model.meshes.size();i++)
                model.meshes[i].remove();
            model.nodes.clear();
            model.meshes.clear();
            model.materials.clear();
        }
        void load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_WARN("Model at: \"" + tPath + "\" doesn't exist");
                return;
            }
            remove();
            LOG_INFO("Loading model at: \"" + tPath + "\"");
            
            auto extBig=String::split(tPath, '.');
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
        }

        void addMesh(const Mesh& tMesh) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tMesh.vertices.size(), tMesh.indices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMesh.material,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tMesh);
            model.nodes.emplace_back("Node_"+std::to_string(model.nodes.size()),Transform(),std::vector<Node>(),static_cast<int>(model.nodes.size()));
        }
        void addMesh(const std::vector<Vertex>& tVertices,
            const std::vector<unsigned int>& tIndices, Material* tMaterial) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tVertices.size(), tIndices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMaterial,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tVertices,tIndices,tMaterial);
            model.nodes.emplace_back("Node_"+std::to_string(model.nodes.size()),Transform(),std::vector<Node>(),static_cast<int>(model.nodes.size()));
        }

        Transform transform;
        Model model;
    private:
        void drawNode(const Node& tNode, const glm::mat4& tParentModel) {
            const glm::mat4 nodeMatrix=tParentModel*tNode.transform.getMatrix();
            if(tNode.index>=0&&tNode.index<static_cast<int>(model.meshes.size()))
                model.meshes[tNode.index].draw(nodeMatrix);
            for(size_t n=0;n<tNode.children.size();n++)
                drawNode(tNode.children[n],nodeMatrix);
        }
    };
}

#endif // ! FS_ENTITY_H