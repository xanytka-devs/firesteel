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
#include <component.hpp>

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

        // Sends update callback to all components.
        virtual void update() {
#ifndef FS_NO_COMPONENTS
            for(size_t i=0;i<mComponents.size();i++) mComponents[i]->onUpdate();
#endif // !FS_NO_COMPONENTS
        }
        // Renders the model with given material or default shader.
        virtual void draw() {
            for(size_t i=0;i<mComponents.size();i++) mComponents[i]->onDraw();
            if(!hasModel()) return;
            for(size_t n=0;n<model.nodes.size();n++)
                drawNode(model.nodes[n], transform.getMatrix());
        }
        // Renders only the given node tree with given material or default shader and parent matrix.
        virtual void drawNode(const std::shared_ptr<Node>& tNode, const glm::mat4& tParentModel, const bool& tOverrideMaterial=false) {
            if(!hasModel()) return;
            const glm::mat4 nodeMatrix=tParentModel*tNode->transform.getMatrix();
            if(tNode->index>=0&&tNode->index<static_cast<int>(model.meshes.size()))
                model.meshes[tNode->index].draw(nodeMatrix,tOverrideMaterial);
            for(size_t n=0;n<tNode->children.size();n++)
                drawNode(tNode->children[n],nodeMatrix,tOverrideMaterial);
        }
        // Replaces materials with default shader with given shader.
        void setMaterialsShader(std::shared_ptr<Shader> tShader, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(size_t m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->getId()==Shader::getDefaultShader()->getId()||tReplaceAll) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces materials with shader id with given shader.
        void replaceMaterialsShader(const unsigned int& tIdToReplace, std::shared_ptr<Shader> tShader) {
            if(!hasModel()) return;
            for(size_t m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->getId()==tIdToReplace) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Replaced entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces all materials with given one.
        void setMaterial(Material* tMaterial, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(size_t m = 0; m < model.materials.size(); m++)
                if(model.materials[m].getShader()->getId()==Shader::getDefaultShader()->getId()||tReplaceAll) model.materials[m]=*tMaterial;
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity material");
#endif // FS_PRINT_DEBUG_MSGS
        }

        bool hasModel() const { return model.meshes.size()!=0; }
        virtual void remove() {
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed entity");
#endif // FS_PRINT_DEBUG_MSGS
            for(size_t i=0;i<model.meshes.size();i++)
                model.meshes[i].remove();
            for(size_t i=0;i<mComponents.size();i++) mComponents[i]->onRemove();
            model.nodes.clear();
            model.meshes.clear();
            model.materials.clear();
        }
        virtual void load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_WARN("Model at path \"" + tPath + "\" doesn't exist");
                return;
            }
            remove();
            LOG_INFO("Loading model at path \"" + tPath + "\"");
            
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

            LOG_INFO("Loaded model at path \"" + tPath + "\"");
        }

        virtual void addMesh(const Mesh& tMesh) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tMesh.vertices.size(), tMesh.indices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMesh.material,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tMesh);
            std::shared_ptr<Node> node = std::make_shared<Node>();
            node->name="Node_"+std::to_string(model.nodes.size());
            node->index=static_cast<int>(model.nodes.size());
            model.nodes.emplace_back(node);
        }
        virtual void addMesh(const std::vector<Vertex>& tVertices,
            const std::vector<unsigned int>& tIndices, Material* tMaterial) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tVertices.size(), tIndices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMaterial,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tVertices,tIndices,tMaterial);
            std::shared_ptr<Node> node = std::make_shared<Node>();
            node->name = "Node_" + std::to_string(model.nodes.size());
            node->index = static_cast<int>(model.nodes.size());
            model.nodes.emplace_back(node);
        }
#ifndef FS_NO_COMPONENTS
        template<typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args&&... tArgs) {
            ASSERT((std::is_base_of<Component,T>::value), "Given component must be derived from base Component type");
            auto comp=std::make_shared<T>(this, std::forward<Args>(tArgs)...);
            mComponents.push_back(comp);
            mComponents[mComponents.size()-1]->onStart();
            return comp;
        }
        void addComponent(std::shared_ptr<Component> tComp) {
            mComponents.push_back(tComp);
            mComponents[mComponents.size()-1]->onStart();
        }
        template<typename T>
        std::shared_ptr<T> getComponent(const size_t& tIdx=0) {
            std::shared_ptr<T> comp=nullptr;
            size_t iter=0;
            for(size_t i=0;i<mComponents.size();i++)
                if(typeid(*mComponents[i])==typeid(T)) {
                    comp=mComponents[i];
                    iter++;
                    if(iter>tIdx) break;
                }
            return comp;
        }
        template<typename T>
        bool hasComponent() {
            for(size_t i=0;i<mComponents.size();i++)
                if(typeid(*mComponents[i]) == typeid(T)) return true;
            return false;
        }
        bool hasComponent(const char* tName) {
            for(size_t i=0;i<mComponents.size();i++)
                if(mComponents[i]->getName()==tName) return true;
            return false;
        }
        template<typename T>
        bool removeComponent(const size_t& tIdx=0) {
            size_t iter=0;
            for(size_t i=0;i<mComponents.size();i++)
                if(typeid(*mComponents[i])==typeid(T)) {
                    iter++;
                    if(iter>tIdx) {
                        mComponents.erase(mComponents.begin() + i);
                        return true;
                    }
                }
            return false;
        }
        bool removeComponent(const size_t& tIdx=0) {
            if(tIdx>=mComponents.size()) return false;
            mComponents.erase(mComponents.begin()+tIdx);
            return true;
        }
        std::vector<std::shared_ptr<Component>> getComponents() { return mComponents; }
#endif // !FS_NO_COMPONENTS
#if !defined(FS_NO_JSON) && !defined(FS_NO_COMPONENTS)
        nlohmann::json serialize() const {
            nlohmann::json js;
            js["name"]=name;
            for(int j=0;j<3;j++) js["transform"]["pos"][j]=transform.position[j];
            for(int j=0;j<3;j++) js["transform"]["rot"][j]=transform.rotation[j];
            for(int j=0;j<3;j++) js["transform"]["size"][j]=transform.size[j];
            nlohmann::json comps=nlohmann::json::array();
			for(const auto& c:mComponents) comps.push_back(c->serialize());
            js["components"]=comps;
            return js;
        }
        static std::shared_ptr<Entity> deserialize(const nlohmann::json& tData) {
            Entity e;
            if(tData.contains("name")) e.name=tData["name"];
            if(tData.contains("transform")) {
                if(tData["transform"].contains("pos")) for(int j=0;j<3;j++) e.transform.position[j]=tData["transform"]["pos"][j];
                if(tData["transform"].contains("rot")) for(int j=0;j<3;j++) e.transform.rotation[j]=tData["transform"]["rot"][j];
                if(tData["transform"].contains("size")) for(int j=0;j<3;j++) e.transform.size[j]=tData["transform"]["size"][j];
            }
            if(tData.contains("components"))
                for(const auto& comp:tData["components"]) {
                    const std::string type=comp["type"];
                    if(ComponentRegistry::Instance->contains(type)) {
                        auto c=ComponentRegistry::Instance->get(type)(&e,comp);
                        e.addComponent(c);
                    } else LOG_ERRR("Unknown component type "+type);
                }
            return std::make_shared<Entity>(e);
        }
#endif // !FS_NO_JSON && !FS_NO_COMPONENTS

        Transform transform;
        Model model;
#ifndef FS_NO_SCENES
        std::string name;
#endif // !FS_NO_SCENES
#ifndef FS_NO_COMPONENTS
    private:
        std::vector<std::shared_ptr<Component>> mComponents;
#endif // !FS_NO_COMPONENTS
    };
}

#endif // ! FS_ENTITY_H