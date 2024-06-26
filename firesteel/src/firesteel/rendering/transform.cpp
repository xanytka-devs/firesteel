#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

#include "firesteel/enviroment.hpp"
#include "firesteel/rendering/transform.hpp"

namespace firesteel {
	Component::Component() : m_transform(nullptr) { }

	Component::Component(Transform& t_transform) : m_transform(&t_transform) { }

	Transform Component::get_transform() const { return *m_transform; }

	void Component::set_transform(Transform* t_value) {
		m_transform = t_value;
	}

	std::shared_ptr<Component> ComponentRegistry::component_by_type(const Component& original) {
		auto it = creators_.find(typeid(original));
		if(it != creators_.end()) {
			return it->second(original);
		}
		return nullptr;
	}

	Transform::Transform(glm::vec3 t_pos, glm::vec4 t_rot, glm::vec3 t_size, const char* t_name,
						bool is_instance, Transform* source_instance) :
		position(t_pos), rotation(t_rot), size(t_size), name(t_name),
		m_is_instance(is_instance), m_source_instance(source_instance) { }

	void Transform::initialize(bool t_add_to_scene) {
		for(std::shared_ptr<Component> c : m_components)
			c->initialize();
		if(t_add_to_scene) Enviroment::get_current_scene()->transforms.push_back(this);
		m_is_initialized = true;
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
		(std::string(name) + " (" + std::to_string(instances_amount()) + ")").c_str(), true, this);
		//Duplicate components.
		for(const auto& comp : m_components)
			inst.add_component(comp->clone());
		m_instances.push_back(inst);
		//Enviroment::get_current_scene()->transforms.push_back(&inst);
		return inst;
	}

	void Transform::load(std::string t_path) {
		//Clear old model (if present).
		remove_model();
		//Initialize importer.
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(t_path, aiProcess_Triangulate
			| aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		//Check if scene isn't corrupted.
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOG_ERRR("Couldn't load model at: '", t_path.c_str(), "'.");
			return;
		}
		//Start processing.
		m_model_path = t_path.substr(0, t_path.find_last_of("\\"));
		m_model_file = t_path.substr(t_path.find_last_of("\\"), t_path.length() - t_path.find_last_of("\\"));
		process_node(scene->mRootNode, scene);
	}

	void Transform::set_cubemap(unsigned int t_id) {
		m_material.shader.enable();
		m_material.shader.set_int("use_skybox", (t_id == -1) ? 0 : 1);
		m_meshes[0].set_cubemap(t_id);
	}

	void Transform::add_component(std::shared_ptr<Component> t_comp, bool t_init) {
		t_comp->set_transform(this);
		m_components.push_back(t_comp);
		if(t_init) t_comp->initialize();
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
			if(t_mesh->HasNormals()) {
				vertex.normal = glm::vec3(
					t_mesh->mNormals[i].x,
					t_mesh->mNormals[i].y,
					t_mesh->mNormals[i].z
				);
			}
			if(t_mesh->mTextureCoords[0]) {
				//Textures.
				vertex.uv = glm::vec2(
					t_mesh->mTextureCoords[0][i].x,
					t_mesh->mTextureCoords[0][i].y
				);
				//Tangent.
				vertex.tangent = glm::vec3(
					t_mesh->mTangents[i].x,
					t_mesh->mTangents[i].y,
					t_mesh->mTangents[i].z
				);
				//Bitangent.
				vertex.bitangent = glm::vec3(
					t_mesh->mBitangents[i].x,
					t_mesh->mBitangents[i].y,
					t_mesh->mBitangents[i].z
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
				aiColor4D height(1.0f);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &def);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emis);
				aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &height);
				//Output.
				glm::vec4 def_v = glm::vec4(def.r, def.g, def.b, def.a);
				glm::vec4 spec_v = glm::vec4(spec.r, spec.g, spec.b, spec.a);
				glm::vec4 emis_v = glm::vec4(emis.r, emis.g, emis.b, emis.a);
				glm::vec4 height_v = glm::vec4(height.r, height.g, height.b, height.a);
				return Mesh(vertices, indices, def_v, spec_v, emis_v, height_v);
			}
			//Albedo.
			std::vector<Texture> texs = load_textures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), texs.begin(), texs.end());
			texs.clear();
			//Specular.
			texs = load_textures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), texs.begin(), texs.end());
			texs.clear();
			//Normal.
			texs = load_textures(material, aiTextureType_HEIGHT);
			textures.insert(textures.end(), texs.begin(), texs.end());
			texs.clear();
			//Height.
			texs = load_textures(material, aiTextureType_AMBIENT);
			textures.insert(textures.end(), texs.begin(), texs.end());
			texs.clear();
		}
		//Output.
		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Transform::load_textures(aiMaterial* t_mat, aiTextureType t_type) {
		std::vector<Texture> output;
		//Check each texture.
		for(unsigned int i = 0; i < t_mat->GetTextureCount(t_type); i++) {
			aiString str;
			t_mat->GetTexture(t_type, i, &str);
			//Prevent duplicates.
			bool skip = false;
			for(unsigned int j = 0; j < m_textures.size(); j++) {
				if(std::strcmp(m_textures[j].path, str.C_Str()) == 0) {
					output.push_back(m_textures[j]);
					skip = true;
					break;
				}
			}
			//Textures aren't loaded.
			if(!skip) {
				Texture tex(m_model_path.c_str(), str.C_Str(), t_type);
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

	void Transform::prerender() {
		//Don't render if it's instance.
		if(m_is_instance) return;
		//Call update function in components.
		for(std::shared_ptr<Component> c : m_components)
			c->preupdate();
		for(Transform inst : m_instances)
			for(std::shared_ptr<Component> c : inst.m_components)
				c->preupdate();
	}

	void Transform::render(bool t_update_components) {
		//Don't render if it's instance.
		if(m_is_instance) return;
		//Rener all instances.
		m_material.shader.enable();
		m_material.shader.set_float("time", static_cast<float>(Renderer::get_time()));
		m_material.shader.set_float("delta_time", Enviroment::delta_time);
		if(Enviroment::get_current_scene()->get_camera() != nullptr) {
			m_material.shader.set_mat4("projection", Enviroment::get_current_scene()->get_camera()->get_projection_matrix());
			m_material.shader.set_mat4("view", Enviroment::get_current_scene()->get_camera()->get_view_matrix());
			m_material.shader.set_3_floats("view_pos", Enviroment::get_current_scene()->get_camera()->position);
		}
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
		//Remove material.
		m_material.shader.remove();
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