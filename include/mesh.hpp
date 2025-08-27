#ifndef FS_MESH_H
#define FS_MESH_H
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "common.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "transform.hpp"

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
        /// Mesh Data.
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        glm::vec3 ambient{0.2f}, diffuse{1.f}, specular{0.5f}, emission{0.f};

        Mesh(const std::vector<Vertex>& tVertices, const std::vector<unsigned int>& tIndices, const std::vector<Texture>& tTextures)
            : vertices(tVertices), indices(tIndices), textures(tTextures) {
            makeMesh();
        }

        Mesh(const std::vector<Vertex>& tVertices, const std::vector<unsigned int>& tIndices,
            const glm::vec3 tDiffuse, const glm::vec3 tSpecular, const glm::vec3 tEmission)
            : vertices(tVertices), indices(tIndices),
            diffuse(tDiffuse), specular(tSpecular), emission(tEmission) {
            makeMesh();
        }

        // Shader must be enabled before running this.
        void draw(const Shader* tShader) {
            //Bind appropriate textures.
            size_t diffuseNr=0;
            size_t specularNr=0;
            size_t normalNr=0;
            size_t heightNr=0;
            size_t emisNr=0;
            size_t opacNr=0;
            //Set material values.
            tShader->setVec4("material.ambient", glm::vec4(ambient, 1));
            tShader->setVec4("material.diffuse", glm::vec4(diffuse, 1));
            tShader->setVec4("material.specular", glm::vec4(specular, 1));
            tShader->setVec4("material.emission", glm::vec4(emission, 1));
            tShader->setBool("noTextures", true);
            //Bind textures.
            if(textures.size()>0) {
                tShader->setBool("noTextures", false);
                Texture::unbind();
                for(unsigned int i=0; i < textures.size(); i++) {
                    //Retrieve texture number.
                    size_t number=0;
                    std::string name=textures[i].typeToString();
                    if(name == "diffuse") number=diffuseNr++;
                    else if(name == "specular") number=specularNr++;
                    else if(name == "normal") number=normalNr++;
                    else if(name == "emission") number=emisNr++;
                    else if(name == "height") number=heightNr++;
                    else if(name == "opacity") number=opacNr++;
                    //Now set the sampler to the correct texture unit.
                    textures[i].enable(i);
                    tShader->setInt("material." + name + std::to_string(number), i);
                    tShader->setBool("material." + name + std::to_string(number) + "_isMonochrome", textures[i].isMonochrome);
                    tShader->setBool("material." + name + "_present", true);
                }
                tShader->setBool("material.opacityMask", opacNr > 0);
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
            for (size_t i=0; i < textures.size(); i++)
                textures[i].remove();
            textures.clear();
        }

        bool hasTextures() const { return textures.size() > 0; }
    private:
        // Render data.
        unsigned int mVAO, mVBO, mEBO;

        void makeMesh() {
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
        std::string name;
        Transform transform;
        std::vector<Node> children;
        int index=-1;
    };
}
#endif // !FS_MESH_H