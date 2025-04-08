#ifndef FSE_SCENE
#define FSE_SCENE

#include "../engine/include/common.hpp"
#include "../engine/include/utils/json.hpp"
#include <fstream>
#include "atmosphere.hpp"
#include "utils.hpp"
using namespace Firesteel;

class Scene {
public:
	Scene() {

	}
	Scene(const char* tPath, Cubemap* tSky) {
		mLoaded = load(tPath, tSky);
	}

	void save(const char* tPath, Cubemap* tSky) const {
		nlohmann::json txt;

		txt["version"] = 1;

		glm::vec3 v = atmosphere.directionalLight.ambient;
		txt["atm"]["dLight"]["amb"] = { v.x, v.y, v.z };
		v = atmosphere.directionalLight.diffuse;
		txt["atm"]["dLight"]["diff"] = { v.x, v.y, v.z };
		v = atmosphere.directionalLight.direction;
		txt["atm"]["dLight"]["dir"] = { v.x, v.y, v.z };
		v = atmosphere.directionalLight.specular;
		txt["atm"]["dLight"]["spec"] = { v.x, v.y, v.z };

		v = atmosphere.fog.color;
		txt["atm"]["fog"]["clr"] = { v.x, v.y, v.z };
		txt["atm"]["fog"]["dnst"] = atmosphere.fog.density;
		txt["atm"]["fog"]["equ"] = atmosphere.fog.equation;
		txt["atm"]["fog"]["points"] = { atmosphere.fog.start, atmosphere.fog.end};

		txt["atm"]["cubemap"]["cfg"] = tSky->getCfgFile();
		txt["atm"]["cubemap"]["size"] = tSky->getSize();

		for (size_t i = 0; i < entities.size(); i++) {
			nlohmann::json obj;
			obj["model"] = StrReplace(entities[i]->entity.model.path, '/', '\\');
			obj["name"] = entities[i]->name;
			v = entities[i]->entity.transform.Position;
			obj["pos"] = { v.x, v.y, v.z };
			v = entities[i]->entity.transform.Rotation;
			obj["rot"] = { v.x, v.y, v.z };
			v = entities[i]->entity.transform.Size;
			obj["size"] = { v.x, v.y, v.z };
			obj["mat"] = entities[i]->materialID;
			txt["entities"].push_back(obj);
		}

		std::ofstream o(tPath);
		o << std::setw(4) << txt << std::endl;
	}

	bool load(const char* tPath, Cubemap* tSky) {
		if(!std::filesystem::exists(tPath)) return false;
		std::ifstream ifs(tPath);
		nlohmann::json txt = nlohmann::json::parse(ifs);

		atmosphere.directionalLight.ambient = Vec4FromJson(&txt["atm"]["dLight"]["amb"]);
		atmosphere.directionalLight.diffuse = Vec4FromJson(&txt["atm"]["dLight"]["diff"]);
		atmosphere.directionalLight.direction = Vec4FromJson(&txt["atm"]["dLight"]["dir"]);
		atmosphere.directionalLight.specular = Vec4FromJson(&txt["atm"]["dLight"]["spec"]);

		atmosphere.fog.color = Vec4FromJson(&txt["atm"]["fog"]["clr"]);
		atmosphere.fog.density = txt["atm"]["fog"]["dnst"];
		atmosphere.fog.equation = txt["atm"]["fog"]["equ"];
		glm::vec2 pts = Vec4FromJson(&txt["atm"]["fog"]["points"]);
		atmosphere.fog.start = pts.x;
		atmosphere.fog.end = pts.y;

		if(!txt["atm"]["cubemap"].is_null()) {
			tSky->remove();
			tSky->load(txt["atm"]["cubemap"]["cfg"]);
			tSky->initialize(txt["atm"]["cubemap"]["size"]);
		}

		remove();
		EditorObject obj{};
		for (auto& el : txt["entities"]) {
			obj = EditorObject{ el["name"],
				Entity(el["model"], Vec4FromJson(&el["pos"]), Vec4FromJson(&el["rot"]), Vec4FromJson(&el["size"])),
					Material{}, el["mat"] };
			entities.push_back(std::make_shared<EditorObject>(obj));
		}

		mLoaded = true;
		mPath = tPath;
		return true;
	}
	EditorObject& operator [](int tID) {
		return *entities[tID];
	}

	void draw(Camera* tCam, std::vector<Material>* tMats, Cubemap* tSky) {
		if(!mLoaded) return;
		// Get variables ready.
		glm::mat4 projection = tCam->getProjection(1);
		glm::mat4 view = tCam->getView();
		// Set parameters for materials.
		for (size_t i = 0; i < tMats->size(); i++) {
			tMats->at(i).enable();
			tMats->at(i).setMat4("projection", projection);
			tMats->at(i).setMat4("view", view);
			tMats->at(i).setVec3("viewPos", tCam->pos);
			tMats->at(i).setInt("time", static_cast<int>(glfwGetTime()));
			if (tMats->at(i).type == 0) {
				atmosphere.setParams(&tMats->at(i).shader);
				//tMats->at(i).setInt("lightingType", 2);
				tMats->at(i).setInt("skybox", 11);
				//tMats->at(i).setInt("numPointLights", 0);
				//tMats->at(i).setInt("numSpotLights", 0);
			}
		}
		// Bind and draw cubemap.
		tSky->bind();
		tSky->draw(&tMats->at(3).shader);
		// Draw entities.
		for(size_t i = 0; i < entities.size(); i++)
			entities[i]->draw(&tMats->at(entities[i]->materialID));
	}

	void remove() {
		mLoaded = false;
		for(size_t i = 0; i < entities.size(); i++)
			entities[i]->remove();
	}

	~Scene() {
		remove();
	}

	std::string getPath() const { return mPath; }
	bool isLoaded() const { return mLoaded; }
	/// Forces state of scene to loaded.
	/// WARN: Use of this function may result in crashes.
	void forceLoadState() {
		LOG_WARN("State of scene was forcefully set to loaded. This may result in crashes")
		mLoaded = true;
	}

	Atmosphere atmosphere;	
	std::vector<std::shared_ptr<EditorObject>> entities;
private:
	bool mLoaded = false;
	std::string mPath = "";
};

#endif // !FSE_SCENE
