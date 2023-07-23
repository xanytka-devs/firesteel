#include "XEngine/SceneManager.hpp"

namespace XEngine {

	size_t SceneManager::scenesAmount = 0;

	void SceneManager::addScene(const Scene scene) {
		/*scenes[scenesAmount] = scene;
		scenesAmount++;*/
	}
	void SceneManager::removeScene(const int id) {
		/*const int amount = scenesAmount - 1;
		Scene newA[amount];
		int i = 0;
		for(Scene scene : scenes) {
			if (scene.getID() != id) {
				newA[i] = scene;
				i++;
			}
		}
		scenesAmount--;*/
	}

	void SceneManager::LoadScene(const int id) {

	}
	void SceneManager::UnloadScene(const int id) {

	}

}