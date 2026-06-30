#ifndef FS_NO_SCENES
#ifndef FS_SCENE
#define FS_SCENE

#include <firesteel/entity.hpp>
#ifndef FS_NO_JSON
#include <firesteel/util/json.hpp>
#endif // !FS_NO_JSON

namespace Firesteel {
	struct Scene {
		Scene() {}
#ifndef FS_NO_JSON
		bool load(const std::string& tPath);
		void save(const std::string& tPath, const bool& tPrettyPrint = false) const;
#endif // !FS_NO_JSON
		void update();
		void draw();
		std::shared_ptr<Entity> add(std::shared_ptr<Entity>& tEntity);
		std::shared_ptr<Entity> add(Entity& tEntity);
		std::shared_ptr<Entity> create();
		bool removeAt(const ulong& tId);
		void clear();
		std::vector<std::shared_ptr<Entity>> entities;
	};
}

#endif // !FS_SCENE
#endif // !FS_NO_SCENES