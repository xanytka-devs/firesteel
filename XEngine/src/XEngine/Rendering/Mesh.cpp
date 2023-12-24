#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "XEngine/Rendering/Mesh.hpp"

namespace XEngine {

	/// <summary>
	/// Generates list of vertexes from floats.
	/// </summary>
	/// <param name="t_vertices">Vertices list.</param>
	/// <param name="t_vert_num">Number of vertices.</param>
	/// <returns>List of vertexes.</returns>
	std::vector<Vertex> Vertex::generate_list(float* t_vertices, int t_vert_num) {
		//Setup stride.
		std::vector<Vertex> output(t_vert_num);
		int stride = sizeof(Vertex) / sizeof(float);
		//Iterate throught vertices.
		for (int i = 0; i < t_vert_num; i++) {
			//Parse positions.
			output[i].pos = glm::vec3(
				t_vertices[i * stride + 0],
				t_vertices[i * stride + 1],
				t_vertices[i * stride + 2]
			);
			//Parse normal.
			output[i].normal = glm::vec3(
				t_vertices[i * stride + 3],
				t_vertices[i * stride + 4],
				t_vertices[i * stride + 5]
			);
			//Parse UVs.
			output[i].uv = glm::vec2(
				t_vertices[i * stride + 6],
				t_vertices[i * stride + 7]
			);
		}
		return output;
	}

	Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, std::vector<Texture> t_textures)
		: vertices(t_vertices), indices(t_indices), textures(t_textures) {
		setup();
	}

	Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, aiColor4D t_diffuse, aiColor4D t_specular)
		: vertices(t_vertices), indices(t_indices), diffuse(t_diffuse), specular(t_specular) {
		setup();
	}

	/// <summary>
	/// Sets up buffers.
	/// </summary>
	void Mesh::setup() {
		//Generate buffers.
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		//Bind buffers.
		// VAO //
		glBindVertexArray(vao);
		// VBO //
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		// EBO //
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		//Attribute pointers.
		// POSITIONS //
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Normals //
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// UVs //
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		//Unbind vertex array.
		glBindVertexArray(0);
	}

	/// <summary>
	/// Render mesh.
	/// </summary>
	/// <param name="t_shader">Shader for model.</param>
	void Mesh::render(Shader t_shader) {
		//Load textures.
		unsigned int diffuse_idx = 0;
		unsigned int specular_idx = 0;
		for(unsigned int i = 0; i < textures.size(); i++) {
			//Activate texture.
			glActiveTexture(GL_TEXTURE0 + i);
			//Retrieve texture info.
			std::string name;
			switch (textures[i].type) {
			case aiTextureType_DIFFUSE:
				name = "diffuse" + std::to_string(diffuse_idx++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular" + std::to_string(specular_idx++);
				break;
			}
			//Set shader value and bind texture.
			t_shader.set_int(name, i);
			textures[i].enable();
		}

		//Enable buffers.
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Unbind active texture.
		glActiveTexture(GL_TEXTURE0);
	}

	/// <summary>
	/// Delete mesh (cleanup).
	/// </summary>
	void Mesh::remove() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

}