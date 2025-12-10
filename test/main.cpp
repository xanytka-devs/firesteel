#include "../include/firesteel.hpp"
using namespace Firesteel;

#include "../include/scene.hpp"
#include "../include/components/announcer.hpp"
#include "../include/utils/imgui_utils.hpp"

Scene scene;
size_t selection;

class WindowApp : public Firesteel::App {
	virtual void onInitialize() override {
		LOG("Hello there!");
		ComponentRegistry::Instance=std::make_unique<ComponentRegistry>();
		ComponentRegistry::Instance->append("fs.generic", [](Entity* e, const nlohmann::json& j) {
			return std::make_shared<Component>(e);
		});
		ComponentRegistry::Instance->append("fs.announcer", [](Entity* e, const nlohmann::json& j) {
			return Announcer::deserialize(e,j);
		});
		scene.entities.push_back(std::make_shared<Entity>());
		scene.entities[0]->name="Test";
	}
	virtual void onUpdate() override {
		ImGui::Begin("Scene");
		if(ImGui::Button("Load"))
			scene.load("scene.json");
		ImGui::SameLine();
		if(ImGui::Button("Save"))
			scene.save("scene.json");
		for(size_t i = 0;i<scene.entities.size();i++)
			if(ImGui::MenuItem(Log::formatStr(scene.entities[i]->name+"##%i", i).c_str(),0,selection-1==i)) selection=i+1;
		if(ImGui::Button("+ Create Entity")) {
			scene.entities.push_back(std::make_shared<Entity>());
			scene.entities[scene.entities.size()-1]->name="New Entity";
		}
		ImGui::End();
		ImGui::Begin("Inspector");
		if(selection!=0) {
			Entity* ent=scene.entities[selection-1].get();
			ImGui::Text("Name: %s", ent->name.c_str());
			ImGui::SameLine();
			if(ImGui::Button("X")) {
				scene.removeAt(selection-1);
				selection=0;
			}
			ImGuiUtil::DragFloat3("Position",&ent->transform.position);
			ImGuiUtil::DragFloat3("Rotation",&ent->transform.rotation);
			ImGuiUtil::DragFloat3("Size",&ent->transform.size);
			ImGui::Separator();
			size_t compId=0;
			for(auto& comp : ent->getComponents()) {
				bool open=ImGui::CollapsingHeader((std::string(comp->getName())+"##"+std::to_string(compId)).c_str(),ImGuiTreeNodeFlags_AllowOverlap);
				ImGui::SameLine();
				if(ImGui::Button(("X##"+std::to_string(compId)).c_str())) {
					ent->removeComponent(compId);
					compId--;
				}
				if(open) {
					ImGui::Text(comp->serialize().dump().c_str());
				}
				compId++;
			}
			if(ImGui::BeginMenu("+ Add Component")) {
				for(const auto& [name,factory] : ComponentRegistry::Instance->map()) {
					if(ImGui::MenuItem(name.c_str())) ent->addComponent(factory(ent,{}));
				}
				ImGui::EndMenu();
			}
		}
		ImGui::End();
	}
	virtual void onShutdown() override {
		scene.clear();
	}
};

int main() {
	WindowApp{}.start();
}