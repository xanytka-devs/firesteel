#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "XEngine/Rendering/Transform.hpp"
#include "XEngine/Log.hpp"

namespace XEngine {

	Transform::Transform(glm::vec3 t_pos, glm::vec4 t_rot, glm::vec3 t_size) :
		position(t_pos), rotation(t_rot), size(t_size) { }

	/// <summary>
	/// Initializes model.
	/// </summary>
	void Transform::initialize() { }

	/// <summary>
	/// Loads model.
	/// </summary>
	/// <param name="t_path">Path to model.</param>
	void Transform::load_model(std::string t_path) {
		//Initialize importer.
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(t_path, aiProcess_Triangulate | aiProcess_FlipUVs);
		//Check if scene isn't corrupted.
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOG_ERRR(("Couldn't load model at: '" + t_path + "'.").c_str());
			return;
		}
		//Start processing.
		m_path = t_path.substr(0, t_path.find_last_of("\\"));
		process_node(scene->mRootNode, scene);
	}

	/// <summary>
	/// Processes node.
	/// </summary>
	/// <param name="t_node">Node.</param>
	/// <param name="t_scene">Scene for node.</param>
	void Transform::process_node(aiNode* t_node, const aiScene* t_scene) {
		//Process all meshes.
		for(unsigned int i = 0; i < t_node->mNumMeshes; i++) {
			aiMesh* mesh = t_scene->mMeshes[t_node->mMeshes[i]];
			m_meshes.push_back(process_mesh(mesh, t_scene));
		}
		//Process all child nodes.
		for (unsigned int i = 0; i < t_node->mNumChildren; i++) {
			process_node(t_node->mChildren[i], t_scene);
		}
	}

	/// <summary>
	/// Processes mesh.
	/// </summary>
	/// <param name="t_mesh">Mesh.</param>
	/// <param name="t_scene">Scene for mesh.</param>
	/// <returns>Instance of mesh.</returns>
	Mesh Transform::process_mesh(aiMesh* t_mesh, const aiScene* t_scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		//Vertices.
		for (unsigned int i = 0; i < t_mesh->mNumVertices; i++) {
			Vertex vertex;
			//Positions.
			vertex.pos = glm::vec3(
				t_mesh->mVertices[i].x,
				t_mesh->mVertices[i].y,
				t_mesh->mVertices[i].z
			);
			//Normals.
			vertex.normal = glm::vec3(
				t_mesh->mNormals[i].x,
				t_mesh->mNormals[i].y,
				t_mesh->mNormals[i].z
			);
			//Textures.
			if(t_mesh->mTextureCoords[0]) {
				vertex.uv = glm::vec2(
					t_mesh->mTextureCoords[0][i].x,
					t_mesh->mTextureCoords[0][i].y
				);
			} else vertex.uv = glm::vec2(0.f);
			vertices.push_back(vertex);
		}
		//Process indices.
		for (unsigned int i = 0; i < t_mesh->mNumFaces; i++) {
			aiFace face = t_mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		//Process material.
		if (t_mesh->mMaterialIndex >= 0) {
			aiMaterial* material = t_scene->mMaterials[t_mesh->mMaterialIndex];
			//Does the model even have textures?
			if(material->GetTextureCount(aiTextureType_DIFFUSE) == 0 && material->GetTextureCount(aiTextureType_SPECULAR) == 0) {
				//Diffuse color.
				aiColor4D def(1.0f);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &def);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &def);
				//Output.
				return Mesh(vertices, indices, def, def);
			} else {
				//Albedo.
				std::vector<Texture> diff_maps = load_textures(material, aiTextureType_DIFFUSE);
				textures.insert(textures.end(), diff_maps.begin(), diff_maps.end());
				//Specular.
				std::vector<Texture> spec_maps = load_textures(material, aiTextureType_SPECULAR);
				textures.insert(textures.end(), spec_maps.begin(), spec_maps.end());
			}
		}
		//Output.
		return Mesh(vertices, indices, textures);
	}

	/// <summary>
	/// Load textures from model file.
	/// </summary>
	/// <param name="t_mat">Material with textures.</param>
	/// <param name="t_type">Texture type.</param>
	/// <returns>Texture, if present.</returns>
	std::vector<Texture> Transform::load_textures(aiMaterial* t_mat, aiTextureType t_type) {
		std::vector<Texture> output;
		//Check each texture.
		for (unsigned int i = 0; i < t_mat->GetTextureCount(t_type); i++) {
			aiString str;
			t_mat->GetTexture(t_type, i, &str);
			//Prevent duplicate loading.
			bool skip = false;
			for (unsigned int j = 0; j < m_textures_loaded.size(); j++) {
				if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0) {
					output.push_back(m_textures_loaded[j]);
					skip = true;
					break;
				}
			}
			//Texture not loaded.
			if (!skip) {
				Texture tex(m_path, str.C_Str(), t_type);
				LOG_INFO((tex.path + "\\" + tex.file).c_str());
				output.push_back(tex);
				m_textures_loaded.push_back(tex);
			}
		}
		//Output.
		return output;
	}

	/// <summary>
	/// Render model.
	/// </summary>
	/// <param name="t_shader">Shader for meshes.</param>
	void Transform::render(Shader t_shader) {
		//Set matrix.
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, size);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		t_shader.set_mat4("model", model);
		t_shader.set_float("material.shininess", 0.5f);
		//Render each mesh.
		for (unsigned int i = 0; i < m_meshes.size(); i++)
			m_meshes[i].render(t_shader);
	}

	/// <summary>
	/// Deletes model (cleanup).
	/// </summary>
	void Transform::remove() {
		//Remove each mesh.
		for (unsigned int i = 0; i < m_meshes.size(); i++)
			m_meshes[i].remove();
	}

}