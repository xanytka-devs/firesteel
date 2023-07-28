#pragma once

#include <iostream>
#include <vector>

#include "CES.hpp"
#include "XEngine/Log.hpp"

namespace XEngine {

	class SceneManager {
	public:
		static SceneManager* Instance;

		SceneManager() {
			instantiate();
		}

		void instantiate() {
			if(!instantiated) {
				Instance = this;
				instantiated = true;
			}
			for(auto& s : scenes) s->instantiate();
			LOG_INFO("Scene manager instantiated.");
		}
		void update() { for(auto& s : scenes) s->update(); }

		Scene& addScene() {
			Scene* s = new Scene();
			std::unique_ptr<Scene> sPtr{ s };
			scenes.emplace_back(std::move(sPtr));
			return *s;
		}
		void removeScene(const int id);

		void loadScene(const int id);
		void unloadScene(const int id);
	private:
		bool instantiated = false;
		size_t maxScenes = 128;
		size_t scenesAmount = 0;
		//static bool loadedScenes[];
		std::vector<std::unique_ptr<Scene>> scenes;
	};

}