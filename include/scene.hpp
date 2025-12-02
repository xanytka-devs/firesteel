#ifndef FS_NO_SCENES
#ifndef FS_SCENE
#define FS_SCENE

#include "entity.hpp"
#ifndef FS_NO_JSON
#include "utils/json.hpp"
#endif // !FS_NO_JSON

namespace Firesteel {
#ifndef FS_NO_JSON
	using ComponentFactory=std::function<std::shared_ptr<Component>(Entity*, const nlohmann::json&)>;
#endif // !FS_NO_JSON
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
				for(size_t i=0;i<scene["entities"].size();i++) {
					Entity ent;
					if(scene["entities"][i].contains("name")) ent.name=scene["entities"][i]["name"];
					if(scene["entities"][i].contains("transform")) {
						nlohmann::json t=scene["entities"][i]["transform"];
						ent.transform.position=glm::vec3(t["pos"][0], t["pos"][1], t["pos"][2]);
						ent.transform.position=glm::vec3(t["rot"][0], t["rot"][1], t["rot"][2]);
						ent.transform.position=glm::vec3(t["size"][0], t["size"][1], t["size"][2]);
					}
					entities.push_back(std::make_shared<Entity>(ent));
				}
		}
		void save(const std::string& tPath) const {
			nlohmann::json scene;

			nlohmann::json ents=nlohmann::json::array();
			for(size_t i=0;i<entities.size();i++) ents.push_back(entities[i]->serialize());
			scene["entities"]=ents;

			std::ofstream o(tPath);
			o/*<<std::setw(4)*/<<scene<<std::endl;
			o.close();
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
		}
		void clear() {
			for(size_t i=0;i<entities.size();i++) entities[i]->remove();
			entities.clear();
		}
		std::vector<std::shared_ptr<Entity>> entities;
	};
}

#endif // !FS_SCENE
#endif // !FS_NO_SCENES