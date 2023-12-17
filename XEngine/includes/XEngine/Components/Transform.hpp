#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "XEngine/Rendering/Behaviour.hpp"

namespace XEngine {
	class Transform : public Behaviour {
	public:
		Transform(glm::vec3 t_pos, glm::vec4 t_rot, glm::vec3 t_size)
			: position(t_pos), rotation(t_rot), size(t_size) {}

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 size;

		void initialize() {
            int vert_num = 36;
            float vertices[] = {
                //    POSITIONS				UVs
				-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
            };
            std::vector<unsigned int> indicies(vert_num);
            for (unsigned int i = 0; i < vert_num; i++)
                indicies[i] = i;
            Texture texture("..\\..\\res\\quads_diffusion.png", "quads");
            meshes.push_back(Mesh(Vertex::generate_list(vertices, vert_num), indicies, { texture }));
		}

        void render(Shader t_shader) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::scale(model, size);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.f), glm::vec3(0.5f));
            t_shader.set_mat4("model", model);

            Behaviour::render(t_shader);
        }
	};
}

#endif // TRANSFORM_H