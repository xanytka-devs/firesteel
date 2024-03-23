#include <vector>
#include "xengine/math.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "xengine/rendering/transform.hpp"
#include "xengine/enviroment.hpp"

namespace XEngine {
	Component::Component() : m_transform(nullptr) { }

	Component::Component(Transform& t_transform) : m_transform(&t_transform) { }

	Transform Component::get_transform() const { return *m_transform; }

	void Component::set_transform(Transform* t_value) {
		m_transform = t_value;
	}

	std::shared_ptr<Component> ComponentRegistry::component_by_type(const Component& original) {
		auto it = creators_.find(typeid(original));
		if (it != creators_.end()) {
			return it->second(original);
		}
		return nullptr;
	}

	Transform::Transform(glm::vec3 t_pos, glm::vec4 t_rot, glm::vec3 t_size, std::string t_name,
						bool is_instance, Transform* source_instance) :
		position(t_pos), rotation(t_rot), size(t_size), name(t_name),
		m_is_instance(is_instance), m_source_instance(source_instance) { }

	void Transform::initialize() {
		for(std::shared_ptr<Component> c : m_components)
			c->initialize();
		m_is_initialized = true;
		Enviroment::get_current_scene()->transforms.push_back(this);
	}

	Transform Transform::create_instance(glm::vec3 t_pos,
		glm::vec4 t_rot, glm::vec3 t_size) {
		//Check if limit isn't reached.
		if(m_instances.size() == MAX_INSTANCES) {
			LOG_ERRR("Couldn't create instance because limit is reached.");
			return Transform();
		}
		//Set variables (if needed).
		if(t_pos == glm::vec3(0.f) &&
			t_rot == glm::vec4(glm::vec3(0.f), 1.f) &&
			t_size == glm::vec3(1.f)) {
			t_pos = position;
			t_rot = rotation;
			t_size = size;
		}
		//Create instance.
		Transform inst = Transform(t_pos, t_rot, t_size, 
		name + " (" + std::to_string(instances_amount()) + ")", true, this);
		//Duplicate components.
		for(const auto& comp : m_components)
			inst.add_component(comp->clone());
		m_instances.push_back(inst);
		//Enviroment::get_current_scene()->transforms.push_back(&m_instances[m_instances.size() - 1]);
		return inst;
	}

	int Transform::instances_amount() {
		return static_cast<int>(m_instances.size());
	}

	void Transform::load_model(std::string t_path) {
		//Clear old model (if present).
		remove_model();
		//Initialize importer.
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(t_path, aiProcess_Triangulate | aiProcess_FlipUVs);
		//Check if scene isn't corrupted.
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOG_ERRR("Couldn't load model at: '" + t_path + "'.");
			return;
		}
		//Start processing.
		m_model_path = t_path.substr(0, t_path.find_last_of("\\"));
		m_model_file = t_path.substr(t_path.find_last_of("\\"), t_path.length() - t_path.find_last_of("\\"));
		process_node(scene->mRootNode, scene);
	}

	void Transform::set_material(Material* t_mat) {
		m_material = *t_mat;
	}

	Material Transform::get_material() const {
		return m_material;
	}

	void Transform::add_component(std::shared_ptr<Component> t_comp, bool t_init) {
		t_comp->set_transform(this);
		m_components.push_back(t_comp);
		if(t_init) t_comp->initialize();
	}

	int Transform::components_amount() {
		return static_cast<int>(m_components.size());
	}

	Component Transform::get_component(int t_id) {
		//Checks if ID is valid.
		if(t_id < static_cast<int>(m_components.size())) return *(m_components[t_id].get());
		//If it's not - return empty component.
		return Component();
	}

	void Transform::process_node(aiNode* t_node, const aiScene* t_scene) {
		//Process all meshes.
		for(unsigned int i = 0; i < t_node->mNumMeshes; i++) {
			aiMesh* mesh = t_scene->mMeshes[t_node->mMeshes[i]];
			m_meshes.push_back(process_mesh(mesh, t_scene));
		}
		//Process all child nodes.
		for(unsigned int i = 0; i < t_node->mNumChildren; i++) {
			process_node(t_node->mChildren[i], t_scene);
		}
	}

	Mesh Transform::process_mesh(aiMesh* t_mesh, const aiScene* t_scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		//Vertices.
		for(unsigned int i = 0; i < t_mesh->mNumVertices; i++) {
			Vertex vertex{};
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
		for(unsigned int i = 0; i < t_mesh->mNumFaces; i++) {
			aiFace face = t_mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		//Process material.
		if(t_mesh->mMaterialIndex >= 0) {
			aiMaterial* material = t_scene->mMaterials[t_mesh->mMaterialIndex];
			//Does the model even have textures?
			if(material->GetTextureCount(aiTextureType_DIFFUSE) == 0
				&& material->GetTextureCount(aiTextureType_SPECULAR) == 0) {
				m_no_textures = true;
				//Diffuse and specular color.
				aiColor4D def(1.0f);
				aiColor4D spec(1.0f);
				aiColor4D emis(1.0f);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &def);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emis);
				//Output.
				glm::vec4 def_v = glm::vec4(def.r, def.g, def.b, def.a);
				glm::vec4 spec_v = glm::vec4(spec.r, spec.g, spec.b, spec.a);
				glm::vec4 emis_v = glm::vec4(emis.r, emis.g, emis.b, emis.a);
				return Mesh(vertices, indices, def_v, spec_v, emis_v);
			}
			//Albedo.
			std::vector<Texture> diff_maps = load_textures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diff_maps.begin(), diff_maps.end());
			//Specular.
			std::vector<Texture> spec_maps = load_textures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), spec_maps.begin(), spec_maps.end());
		}
		//Output.
		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Transform::load_textures(aiMaterial* t_mat, aiTextureType t_type) {
		std::vector<Texture> output;
		//Check each texture.
		for (unsigned int i = 0; i < t_mat->GetTextureCount(t_type); i++) {
			aiString str;
			t_mat->GetTexture(t_type, i, &str);
			//Prevent duplicate loading.
			bool skip = false;
			for (unsigned int j = 0; j < m_textures.size(); j++) {
				if (std::strcmp(m_textures[j].path.data(), str.C_Str()) == 0) {
					output.push_back(m_textures[j]);
					skip = true;
					break;
				}
			}
			//Texture not loaded.
			if (!skip) {
				Texture tex(m_model_path, str.C_Str(), t_type);
				output.push_back(tex);
				m_textures.push_back(tex);
			}
		}
		//Output.
		return output;
	}

	void Transform::draw_instance(Shader t_shader, Transform inst, bool t_update_components) {
		//Call update function in components.
		if(t_update_components)
			for(std::shared_ptr<Component> c : inst.m_components)
				c->update();
		//Set matrix.
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, inst.position);
		model = glm::scale(model, inst.size);
		model = glm::rotate(model, glm::radians(inst.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(inst.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(inst.rotation.z), glm::vec3(0, 0, 1));
		t_shader.set_mat4("model", model);
		//Render each mesh.
		for(unsigned int i = 0; i < m_meshes.size(); i++)
			m_meshes[i].render(t_shader);
	}

	void Transform::render(bool t_update_components) {
		//Don't render if it's instance.
		if(m_is_instance) return;
		//Rener all instances.
		m_material.shader.enable();
		draw_instance(m_material.shader, *this, t_update_components);
		for(Transform inst : m_instances)
			draw_instance(m_material.shader, inst, t_update_components);
	}

	void Transform::remove_model() {
		//Remove each mesh.
		for(unsigned int i = 0; i < m_meshes.size(); i++)
			m_meshes[i].remove();
		m_meshes.clear();
		//Remove each texture.
		for(unsigned int i = 0; i < m_textures.size(); i++)
			m_textures[i].remove();
		m_textures.clear();
	}

	void Transform::remove() {
		//Remove components.
		for(std::shared_ptr<Component> c : m_components)
			c->on_destroy();
		m_components.clear();
		if(m_is_instance) return;
		remove_model();
		//Remove all instances.
		for(unsigned int i = 0; i < m_instances.size(); i++)
			m_instances[i].remove();
		m_instances.clear();
	}

}