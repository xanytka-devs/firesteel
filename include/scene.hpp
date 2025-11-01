#ifndef FS_NO_SCENES
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
				}
		}
		void save(const std::string& tPath) const {
			nlohmann::json scene;
			for(size_t i=0;i<entities.size();i++) {
				scene["entities"][i]["name"]=entities[i]->name;
				for(size_t j=0;j<3;j++) scene["entities"][i]["transform"]["pos"][j]=entities[i]->transform.position[j];
				for(size_t j=0;j<3;j++) scene["entities"][i]["transform"]["rot"][j]=entities[i]->transform.rotation[j];
				for(size_t j=0;j<3;j++) scene["entities"][i]["transform"]["size"][j]=entities[i]->transform.size[j];
			}
			std::ofstream o(tPath);
			o<<scene<<std::endl;
			o.close();
		}
#endif // !FS_NO_JSON
		std::vector<std::shared_ptr<Entity>> entities;
	};
}

#endif // !FS_SCENE
#endif // !FS_NO_SCENES