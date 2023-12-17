#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "XEngine/Rendering/Shader.hpp"
#include "XEngine/Rendering/Texture.hpp"

namespace XEngine {
	struct Vertex {
		static std::vector<struct Vertex> generate_list(float* t_vertices, int t_vert_num);

		glm::vec3 pos;
		glm::vec2 uv;
	};
	typedef struct Vertex Vertex;

	class Mesh {
	public:
		Mesh();
		Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indicies, std::vector<Texture> t_textures);

		void render(Shader shader);
		void remove();

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indicies;
		unsigned int vao;
		std::vector<Texture> textures;
	private:
		unsigned int vbo, ebo;

		void setup();
	};
}

#endif // MESH_H