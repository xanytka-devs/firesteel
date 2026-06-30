#ifndef FS_ENTITY_H
#define FS_ENTITY_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include <firesteel/common.hpp>
#include <firesteel/rendering/mesh.hpp>
#ifndef FS_NO_COMPONENTS
#include <firesteel/component.hpp>
#endif // !FS_NO_COMPONENTS

namespace Firesteel {
    class Entity {
    public:
        Entity(const glm::vec3 tPos = glm::vec3(0), const glm::vec3 tRot = glm::vec3(0), const glm::vec3 tSize = glm::vec3(1));
        Entity(
#if !defined(FS_NO_COMPONENTS)&&!defined(FS_COMPONENT_RENDERING)
            const std::string& tPath,
#endif
            const glm::vec3 tPos = glm::vec3(0), const glm::vec3 tRot = glm::vec3(0), const glm::vec3 tSize = glm::vec3(1));

        // Sends update callback to all components.
        virtual void update();
        // Renders the model with given material or default shader.
        virtual void draw();
#if !defined(FS_NO_COMPONENTS)&&!defined(FS_COMPONENT_RENDERING)
        // Renders only the given node tree with given material or default shader and parent matrix.
        virtual void drawNode(const std::shared_ptr<Node>& tNode, const glm::mat4& tParentModel, const bool& tOverrideMaterial = false);
        void replaceMaterials(std::shared_ptr<Material> tMaterial, const bool& tReplaceAll = false);
        // Replaces materials with default shader with given shader.
        void setMaterialsShader(std::shared_ptr<Shader> tShader, const bool tReplaceAll = false);
        // Replaces materials with shader id with given shader.
        void replaceMaterialsShader(const unsigned int& tIdToReplace, std::shared_ptr<Shader> tShader);

        bool hasModel() const { return model.meshes.size()!=0; }
        virtual void removeModel(const bool& tRemoveMaterials = false);
        virtual bool load(const std::string& tPath);

        virtual void emplaceMesh(const Mesh& tMesh);
        virtual void pushMesh(const Mesh& tMesh);
        virtual void addMesh(const std::vector<Vertex>& tVertices,
            const std::vector<unsigned int>& tIndices, std::shared_ptr<Material> tMaterial);
#endif // FS_NO_COMPONENTS
        virtual void remove();
#ifndef FS_NO_COMPONENTS
        template<typename T, typename... Args>
        static std::shared_ptr<T> addComponent(std::shared_ptr<Entity> tEntity, Args&&... tArgs);
        void addComponent(std::shared_ptr<Component> tComp);
        template<typename T, typename=std::enable_if_t<std::is_base_of_v<Component, T>>>
        std::shared_ptr<T> getComponent(const uint& tIdx = 0);
        template<typename T, typename=std::enable_if_t<std::is_base_of_v<Component, T>>>
        bool hasComponent();
        bool hasComponent(const std::string& tName);
        bool hasComponent(const char* tName);
        template<typename T, typename=std::enable_if_t<std::is_base_of_v<Component, T>>>
        bool removeComponent(const uint& tIdx = 0);
        bool removeComponent(const uint& tIdx = 0);
        template<typename T>
        std::shared_ptr<T> getComponentFromBase(const uint& tIdx = 0);
        template<typename T>
        std::vector<std::shared_ptr<T>> getAllComponentsFromBase();
        template<typename T>
        bool hasComponentFromBase();
        std::vector<std::shared_ptr<Component>> getComponents() { return mComponents; }
        uint getComponetCount() const { return static_cast<uint>(mComponents.size()); }
#endif // !FS_NO_COMPONENTS
#if !defined(FS_NO_JSON) || !defined(FS_NO_COMPONENTS)
        nlohmann::json serialize() const;
        static std::shared_ptr<Entity> deserialize(const nlohmann::json& tData);
#endif // !FS_NO_JSON && !FS_NO_COMPONENTS

        Transform transform;
#ifndef FS_NO_SCENES
        std::string name;
        bool enabled=true;
#endif // !FS_NO_SCENES
#if !defined(FS_NO_COMPONENTS)&&!defined(FS_COMPONENT_RENDERING)
        Model model;
#endif
#ifndef FS_NO_COMPONENTS
    private:
        std::vector<std::shared_ptr<Component>> mComponents;
#endif // !FS_NO_COMPONENTS
    };
}

#endif // ! FS_ENTITY_H