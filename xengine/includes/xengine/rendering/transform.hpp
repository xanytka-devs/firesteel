#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "xengine/log.hpp"
#include "xengine/component.hpp"
#include "xengine/rendering/mesh.hpp"
#include "xengine/rendering/material.hpp"

constexpr auto MAX_INSTANCES = 128;

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace XEngine {

	class Transform {
	public:
		Transform(glm::vec3 t_pos = glm::vec3(0.f),
			glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.f),
			glm::vec3 t_size = glm::vec3(1.f), std::string t_name = "",
			bool is_instance = false, Transform* source_instance = nullptr);
		void initialize();
		bool is_initialized() const { return m_is_initialized; }
		void load_model(std::string t_path);
		void remove_model();
		std::string get_model_path() const { return m_model_path + m_model_file; }
		void render(Shader t_shader, bool t_update_components = true);
		void remove();

		Transform create_instance(glm::vec3 t_pos = glm::vec3(0.f),
			glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.f),
			glm::vec3 t_size = glm::vec3(1.f));
		int instances_amount();
		bool is_instance() const { return m_is_instance; }

		Transform operator =(Transform t) {
			position = t.position;
			rotation = t.rotation;
			size = t.size;
			name = t.name;
			m_instances = t.m_instances;
			m_components = t.m_components;
			m_is_initialized = t.m_is_initialized;
			m_is_instance = t.m_is_instance;
			m_source_instance = t.m_source_instance;
			m_no_textures = t.m_no_textures;
			m_material = t.m_material;
			m_meshes = t.m_meshes;
			m_model_path = t.m_model_path;
			m_model_file = t.m_model_file;
			m_textures = t.m_textures;
			return *this;
		}

		void set_material(Material* t_mat);
		Material get_material() const;

		template <typename T, typename... Args>
		std::shared_ptr<T> add_component(Args&&... args) {
			//Check if valid.
			if(!std::is_base_of<Component, T>::value) {
				LOG_WARN("T must be derived from Component.");
				return std::make_shared<T>();
			}
			//Create component with given args.
			auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
			m_components.push_back(component);
			//Return shared pointer.
			return component;
		}
		void add_component(std::shared_ptr<Component> t_comp, bool t_init = true);
		int components_amount();
		Component get_component(int t_id);
		template <typename T>
		T& get_component() const {
			static T default_component;
			//Check if valid.
			if (!std::is_base_of<Component, T>::value) {
				LOG_WARN("T must be derived from Component.");
				return default_component;
			}
			//Search for component.
			for(const auto& component : m_components)
				if(typeid(*component) == typeid(T))
					return dynamic_cast<T&>(*(component.get()));
			//If component not found - return empty constructor.
			return default_component;
		}

		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 size;
		std::string name;
	protected:
		//Global attributes.
		std::vector<Transform> m_instances;
		std::vector<std::shared_ptr<Component>> m_components;
		//Local attributes.
		bool m_is_initialized = false;
		bool m_is_instance;
		Transform* m_source_instance;
		bool m_no_textures = false;
		Material m_material;
		std::vector<Mesh> m_meshes;
		std::string m_model_path;
		std::string m_model_file;
		std::vector<Texture> m_textures;
		//Functions.
		void process_node(aiNode* t_node, const aiScene* t_scene);
		Mesh process_mesh(aiMesh* mesh, const aiScene* t_scene);
		std::vector<Texture> load_textures(aiMaterial* t_mat, aiTextureType t_type);
		void draw_instance(Shader t_shader, Transform inst, bool t_override_model);
	};
}

#endif // TRANSFORM_H