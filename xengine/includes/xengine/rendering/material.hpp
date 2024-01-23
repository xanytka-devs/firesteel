#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace XEngine {
    struct Material {
        glm::vec4 ambient = glm::vec4(0);
        glm::vec4 diffuse = glm::vec4(1);
        glm::vec4 specular = glm::vec4(0);
        glm::vec4 emission = glm::vec4(0);
        glm::vec4 emission_color = glm::vec4(0);
        float emission_factor = 0.f;
        float shininess = 0.5f;
    };
}

#endif // MATERIAL_H