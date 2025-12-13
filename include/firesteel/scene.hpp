#ifndef FS_NO_SCENES
#ifndef FS_SCENE
#define FS_SCENE

#include <firesteel/entity.hpp>
#ifndef FS_NO_JSON
#include <firesteel/utils/json.hpp>
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
			clear();
			std::ifstream ifs(tPath);
			nlohmann::json scene=nlohmann::json::parse(ifs);
			ifs.close();
			if(scene.contains("entities"))
				for(size_t i=0;i<scene["entities"].size();i++)
					entities.push_back(Entity::deserialize(scene["entities"][i]));
#ifdef FS_PRINT_DEBUG_MSGS
			LOGF_DBG("Loaded %d entities from \"%s\"",
				entities.size(), tPath.c_str());
#endif // FS_PRINT_DEBUG_MSGS
		}
		void save(const std::string& tPath) const {
			nlohmann::json scene;

			nlohmann::json ents=nlohmann::json::array();
			for(size_t i=0;i<entities.size();i++) ents.push_back(entities[i]->serialize());
			scene["entities"]=ents;

			std::ofstream o(tPath);
			o/*<<std::setw(4)*/<<scene<<std::endl;
			o.close();
#ifdef FS_PRINT_DEBUG_MSGS
			LOGF_DBG("Saved %d entities to \"%s\"",
				entities.size(), tPath.c_str());
#endif // FS_PRINT_DEBUG_MSGS
		}
#endif // !FS_NO_JSON
		void update() {
			for(size_t i=0;i<entities.size();i++) entities[i]->update();
		}
		bool removeAt(const size_t& tId) {
			if(tId>=entities.size()) {
				LOGF_WARN("Tried to remove entity at %i while max id is %i",tId,entities.size()-1);
				return false;
			}
			entities[tId]->remove();
			entities.erase(entities.begin()+tId);
			return true;
		}
		void clear() {
#ifdef FS_PRINT_DEBUG_MSGS
			LOGF_DBG("Removed %d entities", entities.size());
#endif // FS_PRINT_DEBUG_MSGS
			for(size_t i=0;i<entities.size();i++) entities[i]->remove();
			entities.clear();
		}
		std::vector<std::shared_ptr<Entity>> entities;
	};
}

#endif // !FS_SCENE
#endif // !FS_NO_SCENES