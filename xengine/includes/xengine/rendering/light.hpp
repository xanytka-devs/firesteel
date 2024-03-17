#ifndef OG_LIGHT_H
#define OG_LIGHT_H

#include <glm/glm.hpp>
#include "xengine/rendering/shader.hpp"

namespace XEngine {
    struct DirectionalLight {
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
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
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
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
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
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

#endif // OG_LIGHT_H