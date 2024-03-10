#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "xengine/rendering/mesh.hpp"

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

	Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices,
		std::vector<Texture> t_textures)
		: vertices(t_vertices), indices(t_indices), textures(t_textures) {
		m_no_textures = (textures.size() == 0);
		setup();
	}

	Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, 
	glm::vec4 t_diffuse, glm::vec4 t_specular, glm::vec4 t_emis)
		: vertices(t_vertices), indices(t_indices),
		diffuse(t_diffuse), specular(t_specular), emission(t_emis) {
		m_no_textures = true;
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
		t_shader.set_4_floats("material.diffuse", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		t_shader.set_4_floats("material.specular", specular.r, specular.g, specular.b, specular.a);
		t_shader.set_4_floats("material.emission", emission.r, emission.g, emission.b, emission.a);
		t_shader.set_int("no_textures", 1);
		if (!m_no_textures) {
			t_shader.set_int("no_textures", 0);
			//Load textures.
			unsigned int diffuse_idx = 0;
			unsigned int specular_idx = 0;
			unsigned int emission_idx = 0;
			for (unsigned int i = 0; i < textures.size(); i++) {
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
				case aiTextureType_EMISSIVE:
					name = "emission" + std::to_string(emission_idx++);
					break;
				}
				//Set shader value and bind texture.
				t_shader.set_int(name, i);
				textures[i].enable();
			}
		}
		//Enable buffers.
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
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
		//Clear arrays.
		vertices.clear();
		indices.clear();
		for(unsigned int i = 0; i < textures.size(); i++)
			textures[i].remove();
		textures.clear();
	}

}