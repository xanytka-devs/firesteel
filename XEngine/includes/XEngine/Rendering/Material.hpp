#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emission;
    float emission_factor;
    float shininess;
};

#endif // MATERIAL_H