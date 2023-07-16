#pragma once

namespace XEngine {

	class Scene {
	public:
		Scene(const char* name);
		~Scene();

		unsigned int getID() { return curID; }

		const char* name;
	private:
		unsigned int curID = 0;
	};

}