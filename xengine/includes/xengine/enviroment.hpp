#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <vector>
#include "math.hpp"
#include "rendering/renderer.hpp"
#include "rendering/transform.hpp"

namespace XEngine {
	class Scene {
	public:
		Scene() : m_id(-1) { }
		Scene(const char* t_name) : m_id(-1), name(t_name) { }

		int get_id() const { return m_id; }
		void set_id(int t_id) { m_id = t_id; }

		std::vector<Transform*> transforms;
		const char* name = "Scene";
	private:
		int m_id;
	};

	class SceneManager {
	public:
		SceneManager() { }

		int get_amount() const { return (int)m_scenes.size(); }

		std::shared_ptr<Scene> add_scene(Scene t_scene) {
			//Check if scene is already on the list.
			if(t_scene.get_id() != -1) {
				for(auto& s : m_scenes)
					if(s->get_id() == t_scene.get_id())
						return nullptr;
			} else t_scene.set_id(m_last_id);
			//Add scene.
			std::shared_ptr<Scene> ptr = std::make_shared<Scene>(t_scene);
			m_scenes.push_back(ptr);
			m_last_id++;
			return ptr;
		}

		Scene* get_scene(int t_id) {
			if(t_id < get_amount() + 1)
				return m_scenes[t_id].get();
			return nullptr;
		}
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		int m_last_id = 0;
	};

	class Enviroment {
	public:
		static glm::vec3 gravity;
		static float delta_time;
		static SceneManager scene_manager;
		static int scene_id;
		static Scene* get_current_scene() { return scene_manager.get_scene(Enviroment::scene_id); }
	};
}

#endif // ENVIROMENT_H