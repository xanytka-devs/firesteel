#ifndef OG_TRANSFORM_H
#define OG_TRANSFORM_H

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
			glm::vec3 t_size = glm::vec3(1.f), std::string t_name = "New Object",
			bool is_instance = false, Transform* source_instance = nullptr);

		/// <summary>
		/// Initializes transform.
		/// </summary>
		/// <param name="t_add_to_scene">Add to scene?</param>
		void initialize(bool t_add_to_scene = true);
		bool is_initialized() const { return m_is_initialized; }
		/// <summary>
		/// Loads transform.
		/// </summary>
		/// <param name="t_path">Path to model.</param>
		void load_model(std::string t_path);
		/// <summary>
		/// Add mesh to model.
		/// </summary>
		/// <param name="t_mesh">Additional mesh.</param>
		void add_mesh(Mesh t_mesh) { m_meshes.push_back(t_mesh); }
		/// <summary>
		/// Removes current model (if present).
		/// </summary>
		void remove_model();
		/// <summary>
		/// Returns full path to model.
		/// </summary>
		/// <returns>Absolute path to model.</returns>
		std::string get_model_path() const { return m_model_path + m_model_file; }
		/// <summary>
		/// Render transform.
		/// </summary>
		/// <param name="t_update_components">Update components?</param>
		void render(bool t_update_components = true);
		/// <summary>
		/// Deletes transform (cleanup).
		/// </summary>
		void remove();

		/// <summary>
		/// Creates instance of this object.
		/// </summary>
		/// <param name="t_pos">New position.</param>
		/// <param name="t_rot">New rotation.</param>
		/// <param name="t_size">New size.</param>
		/// <returns>Instances transform.</returns>
		Transform create_instance(glm::vec3 t_pos = glm::vec3(0.f),
			glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.f),
			glm::vec3 t_size = glm::vec3(1.f));
		/// <summary>
		/// Returns amount of instances.
		/// </summary>
		/// <returns>Amount of instances.</returns>
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

		/// <summary>
		/// Set material.
		/// </summary>
		/// <param name="t_mat">New material.</param>
		void set_material(Material* t_mat);
		/// <summary>
		/// Set material's cubemap ID.
		/// </summary>
		/// <param name="t_id">Cubemap ID (-1 to disable).</param>
		void set_cubemap(unsigned int t_id);
		/// <summary>
		/// Gets material.
		/// </summary>
		/// <returns>Material.</returns>
		Material get_material() const;

		/// <summary>
		/// Add new component by class.
		/// </summary>
		/// <typeparam name="T">Class, derived from Component.</typeparam>
		/// <typeparam name="...Args">Additional initialization variables.</typeparam>
		/// <param name="...args">Additional initialization variables.</param>
		/// <returns>Pointer to new component (optional).</returns>
		template <typename T, typename... Args>
		std::shared_ptr<T> add_component(Args&&... args) {
			//Check if valid.
			if (!std::is_base_of<Component, T>::value) {
				LOG_WARN("T must be derived from Component.");
				return std::make_shared<T>();
			}
			//Create component with given args.
			auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
			m_components.push_back(component);
			//Return shared pointer.
			return component;
		}
		/// <summary>
		/// Add new component by pointer.
		/// </summary>
		/// <param name="t_comp">Pointer to component.</param>
		/// <param name="t_init">Initialize component?</param>
		void add_component(std::shared_ptr<Component> t_comp, bool t_init = true);
		/// <summary>
		/// Gets components amount.
		/// </summary>
		/// <returns>Components amount.</returns>
		int components_amount();
		/// <summary>
		/// Gets component by local ID (index).
		/// </summary>
		/// <param name="t_id">Index of component.</param>
		/// <returns>Component (if present).</returns>
		Component get_component(int t_id);
		/// <summary>
		/// Get component by class.
		/// </summary>
		/// <typeparam name="T">Class, derived from Component.</typeparam>
		/// <returns>Instance of given class from transform (if present).</returns>
		template <typename T>
		T& get_component() const {
			static T default_component;
			//Check if valid.
			if (!std::is_base_of<Component, T>::value) {
				LOG_WARN("T must be derived from Component.");
				return default_component;
			}
			//Search for component.
			for (const auto& component : m_components)
				if (typeid(*component) == typeid(T))
					return dynamic_cast<T&>(*(component.get()));
			//If component not found - return empty constructor.
			return default_component;
		}
		/// <summary>
		/// Get the list of components.
		/// </summary>
		/// <returns>List of components.</returns>
		std::vector<std::shared_ptr<Component>>* get_component_list() { return &m_components; }

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
		/// <summary>
		/// Processes model node.
		/// </summary>
		/// <param name="t_node">Node.</param>
		/// <param name="t_scene">Scene for node.</param>
		void process_node(aiNode* t_node, const aiScene* t_scene);
		/// <summary>
		/// Processes mesh.
		/// </summary>
		/// <param name="t_mesh">Mesh.</param>
		/// <param name="t_scene">Scene for mesh.</param>
		/// <returns>Instance of mesh.</returns>
		Mesh process_mesh(aiMesh* mesh, const aiScene* t_scene);
		/// <summary>
		/// Load textures from model file.
		/// </summary>
		/// <param name="t_mat">Material with textures.</param>
		/// <param name="t_type">Texture type.</param>
		/// <returns>Texture, if present.</returns>
		std::vector<Texture> load_textures(aiMaterial* t_mat, aiTextureType t_type);
		/// <summary>
		/// Method to draw instance of an object.
		/// </summary>
		/// <param name="t_shader">Shader for meshes.</param>
		/// <param name="inst">Instance transform.</param>
		/// <param name="t_update_components">Update components?</param>
		void draw_instance(Shader t_shader, Transform inst, bool t_override_model);
	};
}

#endif // OG_TRANSFORM_H