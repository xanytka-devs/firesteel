#include <glad/glad.h>

#include "XEngine/Rendering/Mesh.hpp"
#include <XEngine/Log.hpp>
#include "XEngine/Rendering/VertexArray.hpp"

namespace XEngine::Rendering {

    Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Material>* materials) {
        //Assign values.
        this->vertices = *vertices;
        this->indices = *indices;
        this->materials = *materials;
        //Setup.
        //setupMesh();
    }

    void Mesh::setupMesh() {
        //Create shader program.
        //Vertex buffers and array.
        BufferLayout bufLayout{
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };
        vao = std::make_unique<VertexArray>();
        vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), bufLayout);
        ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        //Add buffers to array.
        vao->addVertexBuffer(*vbo.get());
        vao->setIndexBuffer(*ibo.get());
    }

    void Mesh::Draw() {
        //TODO: Mesh loading through assimp.
        LOG_INFO("Drawn mesh.");
    }

}