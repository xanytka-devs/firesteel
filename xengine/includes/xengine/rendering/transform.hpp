#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "xengine/component.hpp"
#include "xengine/rendering/mesh.hpp"
#include "xengine/rendering/material.hpp"

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
		void render(Shader t_shader, bool t_override_model = false);
		void remove();

		void set_material(Material* t_mat);
		Material get_material() const;

		template <typename T, typename... Args>
		std::shared_ptr<T> add_component(Args&&... args) {
			//Check if valid.
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			//Create component with given args.
			auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
			m_components.push_back(component);
			//Return shared pointer.
			return component;
		}
		void add_component(std::shared_ptr<Component> t_comp);
		int components_amount();
		Component get_component(int t_id);

		template <typename T>
		T& get_component() const {
			//Check if valid.
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			//Search for component.
			for(const auto& component : m_components)
				if(typeid(*component) == typeid(T))
					return dynamic_cast<T&>(*(component.get()));
			//If component not found - return empty constructor.
			return T();
		}

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 size;
	protected:
		std::vector<std::shared_ptr<Component>> m_components;
		std::vector<Mesh> m_meshes;
		std::string m_path;
		std::vector<Texture> m_textures_loaded;
		Material m_material;
		bool m_no_textures = false;
		glm::mat4 custom_model = glm::mat4(1.0f);

		void process_node(aiNode* t_node, const aiScene* t_scene);
		Mesh process_mesh(aiMesh* mesh, const aiScene* t_scene);
		std::vector<Texture> load_textures(aiMaterial* t_mat, aiTextureType t_type);
		void reset_components();
	};
}

#endif // TRANSFORM_H