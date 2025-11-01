#ifndef FS_SCENE
#define FS_SCENE

#include "entity.hpp"
#ifndef FS_NO_JSON
#include "utils/json.hpp"
#endif // !FS_NO_JSON

namespace Firesteel {
	struct Scene {
		Scene() {}
#ifndef FS_NO_JSON
		void load(const std::string& tPath) {
			if(!std::filesystem::exists(tPath)) {
				LOG_ERRR("Scene at path \""+tPath+"\" doesn't exist");
				return;
			}
			nlohmann::json scene=nlohmann::json::parse(tPath);
			if(scene.contains("entities"))
				for(size_t i=0;i<scene["entities"].size();i++) {

				}
		}
#endif // !FS_NO_JSON
		std::vector<std::shared_ptr<Entity>> entities;
	};
}

#endif // !FS_SCENE