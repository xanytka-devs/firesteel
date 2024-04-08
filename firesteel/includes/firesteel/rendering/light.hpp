#ifndef OG_LIGHT_H
#define OG_LIGHT_H

#include <glm/glm.hpp>
#include "firesteel/rendering/shader.hpp"

namespace firesteel {
    struct DirectionalLight {
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
        void render(Shader t_shader) const;
        //Location.
        glm::vec3 direction = glm::vec3(0);
        //Base variables.
        glm::vec4 ambient = glm::vec4(0);
        glm::vec4 diffuse = glm::vec4(0);
        glm::vec4 specular = glm::vec4(0);
        glm::vec4 color = glm::vec4(1.f);
    };

    struct PointLight {
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
        void render(Shader t_shader, int idx) const;
        //Location.
        glm::vec3 position = glm::vec3(0);
        //Attenuation.
        float k0 = 0;
        float k1 = 0;
        float k2 = 0;
        //Base variables.
        glm::vec4 ambient = glm::vec4(0);
        glm::vec4 diffuse = glm::vec4(0);
        glm::vec4 specular = glm::vec4(0);
        glm::vec4 color = glm::vec4(1.f);
    };

    struct SpotLight {
        /// <summary>
        /// Set lighting values to shader.
        /// </summary>
        /// <param name="t_shader">Shader to set values to.</param>
        void render(Shader t_shader, int idx) const;
        //Location.
        glm::vec3 position = glm::vec3(0);
        glm::vec3 direction = glm::vec3(0);
        //Attenuation.
        float k0 = 0;
        float k1 = 0;
        float k2 = 0;
        //Cutoffs.
        float inner_cutoff = 0;
        float outer_cutoff = 0;
        //Base variables.
        glm::vec4 ambient = glm::vec4(0);
        glm::vec4 diffuse = glm::vec4(0);
        glm::vec4 specular = glm::vec4(0);
        glm::vec4 color = glm::vec4(1.f);
    };
}

#endif // OG_LIGHT_H