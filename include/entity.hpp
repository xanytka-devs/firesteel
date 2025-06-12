#ifndef FS_MODEL_H
#define FS_MODEL_H
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
#include "loaders/obj.hpp"

#include "utils/stbi_global.hpp"

namespace Firesteel {

    class Entity {
    public:
        Transform transform;
        Model model;

        // Simplified constructor.
        Entity(const glm::vec3 tPos = glm::vec3(0), const glm::vec3 tRot = glm::vec3(0), const glm::vec3 tSize = glm::vec3(1)) {
            transform = Transform(tPos, tRot, tSize);
        }

        // Constructor, expects a filepath to a 3D model.
        Entity(const std::string& tPath,
            const glm::vec3 tPos = glm::vec3(0), const glm::vec3 tRot = glm::vec3(0), const glm::vec3 tSize = glm::vec3(1)) {
            transform = Transform(tPos, tRot, tSize);
            load(tPath);
        }

        // Renders the model.
        void draw(const Shader* tShader) {
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
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed entity");
#endif // FS_PRINT_DEBUG_MSGS
            for(size_t i = 0; i < model.meshes.size(); i++)
                model.meshes[i].remove();
            for(size_t i = 0; i < model.materials.size(); i++)
                model.materials[i].remove();
            mHasMeshes = false;
        }

        // Loads a model (if it exists).
        void load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_INFO("Model at: \"" + tPath + "\" doesn't exist");
                return;
            }
            LOG_INFO("Loading model at: \"" + tPath + "\"");
            
            auto extBig = StrSplit(tPath, '.');
            std::string ext = extBig[extBig.size()-1];
            if(ext=="obj") model = OBJ::load(tPath);
            else LOG_ERRR("Looks like \"" + ext + " \" model format isn't supported. Please try obj, gltf or fbx.");

            LOG_INFO("Successfully loaded model at: \"" + tPath + "\"");
            mHasMeshes = true;
        }

    private:
        static glm::mat4 modelMatrix;
        bool mHasMeshes = false;
    };

}

glm::mat4 Firesteel::Entity::modelMatrix = glm::mat4(1);

#endif // ! FS_MODEL_H