#pragma once

#include <string>
#include <vector>

#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"

namespace XEngine::Rendering {

    struct Shader {
        unsigned int id;
        std::string type;
    };

    class Mesh {
    public:
        //Mesh data.
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Material> materials;

        Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Material>* materials);
        void Draw();
    private:
        //Render data.
        std::unique_ptr<VertexArray> vao;
        /*std::unique_ptr<VertexBuffer> vbo;
        std::unique_ptr<IndexBuffer> ibo;*/

        void setupMesh();
    };

}