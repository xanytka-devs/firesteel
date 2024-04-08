#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <vector>
#include "math.hpp"
#include "rendering/renderer.hpp"
#include "rendering/transform.hpp"
#include "rendering/camera.hpp"

namespace firesteel {
	class Scene {
	public:
		Scene() : m_id(-1), m_camera(nullptr) { }
		Scene(const char* t_name, Camera* t_cam) : m_id(-1), name(t_name),
			m_camera(t_cam) { }

		/// <summary>
		/// Get ID of this scene.
		/// </summary>
		/// <returns>ID of this scene.</returns>
		int get_id() const { return m_id; }
		/// <summary>
		/// Set ID to this scene.
		/// </summary>
		/// <param name="t_id">New ID.</param>
		void set_id(int t_id) { m_id = t_id; }

		/// <summary>
		/// Get currently active camera.
		/// </summary>
		/// <returns>Active camera.</returns>
		Camera* get_camera() const { return m_camera; }
		/// <summary>
		/// Set active camera.
		/// </summary>
		/// <param name="t_cam">Active camera.</param>
		void set_camera(Camera* t_cam) { m_camera = t_cam; }

		std::vector<Transform*> transforms;
		const char* name = "Scene";
	private:
		unsigned char m_id;
		Camera* m_camera;
	};

	class SceneManager {
	public:
		SceneManager() { }

		/// <summary>
		/// Get amount of scenes.
		/// </summary>
		/// <returns>Amount of scenes.</returns>
		unsigned char get_amount() const { return (unsigned char)m_scenes.size(); }

		/// <summary>
		/// Add new scene to manager.
		/// </summary>
		/// <param name="t_scene">New scene.</param>
		/// <returns>Pointer to added scene (optional).</returns>
		std::shared_ptr<Scene> add_scene(Scene t_scene) {
			if(!initialized) {
				m_scenes.clear();
				initialized = true;
			}
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

		/// <summary>
		/// Removes all scenes.
		/// </summary>
		void clear_scenes() { m_scenes.clear(); }

		/// <summary>
		/// Get scene by given ID.
		/// </summary>
		/// <param name="t_id">ID of scene to search for.</param>
		/// <returns>Pointer to scene (if found).</returns>
		Scene* get_scene(unsigned char t_id) {
			if(t_id < get_amount() + 1)
				return m_scenes[t_id].get();
			return nullptr;
		}

		bool initialized = false;
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		unsigned char m_last_id = 0;
	};

	class Enviroment {
	public:
		static glm::vec3 gravity;
		static float delta_time;
		static SceneManager scene_manager;
		static unsigned char scene_id;

		/// <summary>
		/// Get current scene.
		/// </summary>
		/// <returns>Current scene.</returns>
		static Scene* get_current_scene() { return scene_manager.get_scene(Enviroment::scene_id); }
	};
}

#endif // ENVIROMENT_H