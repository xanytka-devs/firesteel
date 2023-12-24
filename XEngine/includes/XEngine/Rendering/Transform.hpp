#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "XEngine/Rendering/Mesh.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace XEngine {
	class Transform {
	public:
		Transform(glm::vec3 t_pos = glm::vec3(0.f),
			glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.f),
			glm::vec3 t_size = glm::vec3(1.f));
		void initialize();
		void load_model(std::string t_path);
		void render(Shader t_shader);
		void remove();

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 size;
	protected:
		std::vector<Mesh> m_meshes;
		std::string m_path;
		std::vector<Texture> m_textures_loaded;

		void process_node(aiNode* t_node, const aiScene* t_scene);
		Mesh process_mesh(aiMesh* mesh, const aiScene* t_scene);
		std::vector<Texture> load_textures(aiMaterial* t_mat, aiTextureType t_type);
	};
}

#endif // TRANSFORM_H