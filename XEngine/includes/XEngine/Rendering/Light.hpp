#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "XEngine/Rendering/Shader.hpp"

namespace XEngine {
    struct DirectionalLight {
        void render(Shader t_shader);
        //Location.
        glm::vec3 direction;
        //Base variables.
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 color = glm::vec4(1.f);
    };

    struct PointLight {
        void render(Shader t_shader, int idx);
        //Location.
        glm::vec3 position;
        //Attenuation.
        float k0;
        float k1;
        float k2;
        //Base variables.
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 color = glm::vec4(1.f);
    };

    struct SpotLight {
        void render(Shader t_shader, int idx);
        //Location.
        glm::vec3 position;
        glm::vec3 direction;
        //Attenuation.
        float k0;
        float k1;
        float k2;
        //Cutoffs.
        float inner_cutoff;
        float outer_cutoff;
        //Base variables.
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 color = glm::vec4(1.f);
    };
}

#endif // LIGHT_H