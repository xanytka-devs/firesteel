#ifndef OG_MESH_H
#define OG_MESH_H

#include <vector>
#include "firesteel/math.hpp"

#include "firesteel/rendering/shader.hpp"
#include "firesteel/rendering/texture.hpp"

namespace firesteel {
	struct Vertex {
		/// <summary>
		/// Generates list of vertexes from floats.
		/// </summary>
		/// <param name="t_vertices">Vertices list.</param>
		/// <param name="t_vert_num">Number of vertices.</param>
		/// <returns>List of vertexes.</returns>
		static std::vector<struct Vertex> generate_list(float* t_vertices, int t_vert_num);
		/// <summary>
		/// Calculate tangent vectors for each face.
		/// </summary>
		void Vertex::calc_tan_vectors(std::vector<Vertex>& t_list, std::vector<unsigned int>& t_indices);

		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 tangent;
	};
	typedef struct Vertex Vertex;

	class Mesh {
	public:
		Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices,
			std::vector<Texture> t_textures = {});
		Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices,
			glm::vec4 t_diffuse, glm::vec4 t_specular, glm::vec4 t_emis);

		/// <summary>
		/// Render mesh.
		/// </summary>
		/// <param name="t_shader">Shader for model.</param>
		void render(Shader t_shader);
		/// <summary>
		/// Delete mesh (cleanup).
		/// </summary>
		void remove();
		/// <summary>
		/// Set material's cubemap ID.
		/// </summary>
		/// <param name="t_id">Cubemap ID.</param>
		void set_cubemap(unsigned int t_id);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int vao;
		std::vector<Texture> textures;
		glm::vec4 diffuse{ 1.f };
		glm::vec4 specular{ 0.f };
		glm::vec4 emission{ 0.f };
	private:
		bool m_no_textures = false;
		unsigned int vbo, ebo;
		unsigned int m_cubemap_id = 1;

		/// <summary>
		/// Sets up buffers.
		/// </summary>
		void setup();
	};
}

#endif // OG_MESH_H