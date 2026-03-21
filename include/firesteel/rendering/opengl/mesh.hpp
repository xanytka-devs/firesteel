#ifdef FS_RENDERER_OGL
#ifndef FS_OGL_MESH_H
#define FS_OGL_MESH_H

#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
	struct OGLMesh : public Mesh {
	public:
        OGLMesh(const std::vector<Vertex>& tVertices, const std::vector<uint>& tIndices, std::shared_ptr<Material> tMaterial)
            : Mesh(tVertices,tIndices,tMaterial) {
            makeMesh();
        }
    protected:
        void mDraw() override {
            glBindVertexArray(mVAO);
            glDrawElements(GL_TRIANGLES, static_cast<uint>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        void mRemove() override {
            glDeleteVertexArrays(1, &mVAO);
        }
    private:
        uint mVAO;
	private:
        void makeMesh() {
            uint mVBO, mEBO;
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
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

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
}

#endif // !FS_OGL_MESH_H
#endif // FS_RENDERER_OGL