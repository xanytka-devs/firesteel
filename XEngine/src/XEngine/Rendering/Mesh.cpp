#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
			//Parse UVs.
			output[i].uv = glm::vec2(
				t_vertices[i * stride + 3],
				t_vertices[i * stride + 4]
			);
		}
		return output;
	}

	Mesh::Mesh() { }

	/// <summary>
	/// Creates instance of model.
	/// </summary>
	/// <param name="t_vertices">Vertices list.</param>
	/// <param name="t_indicies">Indicies list.</param>
	/// <param name="t_textures">Textures list.</param>
	Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indicies, std::vector<Texture> t_textures)
		: vertices(t_vertices), indicies(t_indicies), textures(t_textures) {
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);
		//Attribute pointers.
		// POSITIONS //
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// UVs //
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		//Unbind vertex array.
		glBindVertexArray(0);
	}

	/// <summary>
	/// Render mesh.
	/// </summary>
	/// <param name="shader">Shader for model.</param>
	void Mesh::render(Shader shader) {
		//Go throught textures and set them in shader.
		for (unsigned int i = 0; i < textures.size(); i++) {
			shader.set_int(textures[i].name, textures[i].id);
			textures[i].enable();
		}
		//Enable buffers.
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
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