#pragma once

#include "CES.hpp"

namespace XEngine {

	class SceneManager {
	public:
		static void addScene(const Scene scene);
		static void removeScene(const int id);

		static void LoadScene(const int id);
		static void UnloadScene(const int id);
	private:
		static size_t scenesAmount;
		static bool loadedScenes[];
		static Scene scenes[];
	};

}