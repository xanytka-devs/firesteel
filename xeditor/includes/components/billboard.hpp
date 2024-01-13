#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <string>
#include <vector>

#include <xengine/utils.hpp>
#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/transform.hpp>
#include <xengine/rendering/camera.hpp>

class Billboard : public XEngine::Transform {
public:
    XEngine::Camera* m_cam;
    std::string m_sprite = "";
    std::string m_file = "";

    Billboard(glm::vec3 t_pos, glm::vec4 t_rot, 
       glm::vec3 t_size, XEngine::Camera* t_cam, std::string t_sprite)
        : XEngine::Transform(t_pos, t_rot, t_size), m_cam(t_cam) {
        std::vector<std::string> file = XEngine::Utils::split_str(&t_sprite, '\\');
        for(int i = 0; i < file.size(); i++) {
            if(i == file.size() - 1) m_file = file[i];
            else m_sprite += file[i] + '\\';
        }
    }

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
        m_meshes.push_back(XEngine::Mesh(XEngine::Vertex::generate_list(vertices, vert_num), indicies/*,
           { XEngine::Texture(m_sprite, m_file, aiTextureType(1))}*/));
    }

    glm::vec4 cam_rot = (*m_cam).rotation;
    void render(XEngine::Shader t_shader) {
        Transform::position = position;
        // Получение позиции камеры
        glm::vec3 cameraPos = m_cam->position;
        // Создание вектора, направленного от объекта к камере
        glm::vec3 facing = glm::normalize(cameraPos - position);
        // Вычисление новой матрицы вида, ориентированной в сторону камеры
        glm::mat4 viewMatrix = glm::lookAt(position, cameraPos, m_cam->up);
        // Отключение вращения объекта вокруг своей оси
        glm::mat4 noRotationMatrix = glm::mat4(1.0f);
        noRotationMatrix = glm::mat4(glm::mat3(viewMatrix));
        // Применение новой матрицы вида без вращения к объекту
        custom_model = noRotationMatrix;
        // Рендеринг объекта
        Transform::render(t_shader, true);
    }
};

#endif // BILLBOARD_H