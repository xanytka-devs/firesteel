#ifndef FS_MESH_H
#define FS_MESH_H
#include <glm/gtc/matrix_transform.hpp>

#include <firesteel/transform.hpp>
#include <firesteel/rendering/material.hpp>

namespace Firesteel {
#define MAX_BONE_INFLUENCE 4
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        //Bone indexes which will influence this vertex.
        int boneIDs[MAX_BONE_INFLUENCE];
        //Weights from each bone.
        float boneWeights[MAX_BONE_INFLUENCE];
    };

    struct Mesh {
    public:
        Mesh(const std::vector<Vertex>& tVertices, const std::vector<unsigned int>& tIndices, Material* tMaterial)
            : vertices(tVertices), indices(tIndices), material(tMaterial) {
            makeMesh();
        }
        // If `tOverrideMaterial` is enbaled then shader must be enabled before running this.
        void draw(const glm::mat4& tModel, const bool& tOverrideMaterial=false) {
            if (!tOverrideMaterial) {
                material->bind();
                material->getShader()->setMat4("model", tModel);
            }
            //Draw mesh.
            glBindVertexArray(mVAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            //Always good practice to set everything back to defaults once configured.
            Texture::unbind();
        }
        void remove() {
            glDeleteVertexArrays(1, &mVAO);
            //Clear mesh data.
            vertices.clear();
            indices.clear();
        }
    public:
        /// Mesh Data.
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material* material;
    protected:
        // Render data.
        unsigned int mVAO;
    private:
        void makeMesh() {
            unsigned int mVBO, mEBO;
            //Create buffers/arrays.
            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glGenBuffers(1, &mEBO);
            glBindVertexArray(mVAO);
            //Load data into vertex buffers.
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            //Load data into entity buffers.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //Set the vertex attribute pointers.
            //Positions.
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            //Normals.
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            //UVs.
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            //Tangent.
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
            //Bitangent.
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
            //Bones.
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
            //Bone weights.
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));
            glBindVertexArray(0);
            //Delete unnecessary buffers (they've been copied to VAO).
            glDeleteBuffers(1, &mEBO);
            glDeleteBuffers(1, &mVBO);
        }
    };
    struct Node {
        Node() :
            name("New Node"), transform(Transform{}),
            children(std::vector< std::shared_ptr<Node>>()), parent(nullptr) {}
        Node(const std::string& tName, const Transform& tTransform, const std::vector<std::shared_ptr<Node>>& tChildren, std::shared_ptr<Node> tParent, const int& tIndex)
            : name(tName), transform(tTransform), children(tChildren), index(tIndex), parent(tParent) {}
        
        glm::mat4 getMatrix(glm::mat4 tEntityMatrix) const {
            glm::mat4 model=tEntityMatrix*transform.getMatrix();
            std::shared_ptr<Node> lastParent=parent;
            while(lastParent) {
                model*=lastParent->transform.getMatrix();
                lastParent=lastParent->parent;
            }
            return model;
        }
        
        std::string name;
        Transform transform;
        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Node> parent=nullptr;
        int index=-1;
    };
    struct Model {
        Model(const std::string& tPath="") {
            materials.clear();
            meshes.clear();
            path=tPath;
        }
        std::string getDirectory() const {
            return path.substr(0, path.find_last_of('\\'));
        }
        std::string getFilename() const {
            return path.substr(path.find_last_of('\\')+1, path.size()-path.find_last_of('\\')-1);
        }

        std::string path;
        std::vector<Material> materials;
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<Mesh> meshes;
    };
}
#endif // !FS_MESH_H