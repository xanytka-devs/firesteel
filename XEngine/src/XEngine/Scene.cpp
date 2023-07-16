#include "Scene.hpp"

namespace XEngine {

	Scene::Scene(const char* name)
	: name(name) {

	}

	Scene::~Scene() {
		curID = 0;
	}

}