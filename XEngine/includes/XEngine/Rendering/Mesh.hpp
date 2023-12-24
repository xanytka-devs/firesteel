#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

#include "XEngine/Rendering/Shader.hpp"
#include "XEngine/Rendering/Texture.hpp"
#include <assimp/postprocess.h>

namespace XEngine {
	struct Vertex {
		static std::vector<struct Vertex> generate_list(float* t_vertices, int t_vert_num);

		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	typedef struct Vertex Vertex;

	class Mesh {
	public:
		Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, std::vector<Texture> t_textures = {});
		Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, aiColor4D t_diffuse, aiColor4D t_spec);

		void render(Shader t_shader);
		void remove();

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int vao;
		std::vector<Texture> textures;
		aiColor4D diffuse;
		aiColor4D specular;
	private:
		bool m_no_textures = false;
		unsigned int vbo, ebo;

		void setup();
	};
}

#endif // MESH_H