#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "XEngine/Rendering/Renderer.hpp"
#include "XEngine/Rendering/Behaviour.hpp"
#include "XEngine/Rendering/Material.hpp"

namespace XEngine {
	class Transform : public Behaviour {
	public:
        Transform() { }
		Transform(glm::vec3 t_pos, glm::vec4 t_rot, glm::vec3 t_size, Material t_mat)
			: position(t_pos), rotation(t_rot), size(t_size), material(t_mat) {}

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 size;
        bool shoud_rotate = true;

        Material material;

		void initialize() {
			unsigned int vert_num = 36;
            float vertices[] = {
            //       POSITIONS                 NORMAL               UVs
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
            };
            std::vector<unsigned int> indicies(vert_num);
            for (unsigned int i = 0; i < vert_num; i++)
                indicies[i] = i;
            Texture texture_albedo("..\\..\\res\\quads_diffusion.png", "material.diffuse");
            Texture texture_specular("..\\..\\res\\quads_specular.png", "material.specular");
            Texture texture_emission("..\\..\\res\\quads_emission.png", "material.emission");
            mesh = Mesh(Vertex::generate_list(vertices, vert_num), indicies, { texture_albedo, texture_specular, texture_emission });
		}

        void render(Shader t_shader) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::scale(model, size);
            if(shoud_rotate)
                model = glm::rotate(model, (float)Renderer::get_time() * glm::radians(-55.f), glm::vec3(0.5f));
            else {
                model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
                model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
                model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
            }
            t_shader.set_mat4("model", model);
            for (size_t i = 0; i < mesh.textures.size(); i++)
                mesh.textures[i].enable();
            t_shader.set_3_floats("material.ambient", material.ambient);
            t_shader.set_float("material.shininess", material.shininess);
            t_shader.set_float("material.emission_factor", material.emission_factor);
            t_shader.set_3_floats("material.emission_color", material.emission);

            Behaviour::render(t_shader);
        }
	};
}

#endif // TRANSFORM_H