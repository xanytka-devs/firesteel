#ifndef CORD_PLANE_H
#define CORD_PLANE_H

#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/transform.hpp>
#include <xengine/rendering/material.hpp>

class CordPlane : public XEngine::Transform {
public:
    CordPlane(glm::vec3 t_pos = glm::vec3(0.0f), glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.0f), glm::vec3 t_size = glm::vec3(1.0f))
        : XEngine::Transform(t_pos, t_rot, t_size) {}

    void initialize() {
        unsigned int vert_num = 6;
        float vertices[] = {
            //       POSITIONS                 NORMAL               UVs
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f
        };
        std::vector<unsigned int> indicies(vert_num);
        for (unsigned int i = 0; i < vert_num; i++)
            indicies[i] = i;
        m_meshes.push_back(XEngine::Mesh(XEngine::Vertex::generate_list(vertices, vert_num), indicies));
    }

    void render(XEngine::Shader t_shader) {
        Transform::render(t_shader);
    }
};

#endif // CORD_PLANE_H