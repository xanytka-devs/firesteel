#include "../include/firesteel.hpp"
using namespace Firesteel;

#include "../include/scene.hpp"
#include "../include/utils/imgui_utils.hpp"

Scene scene;
size_t selection;

class WindowApp : public Firesteel::App {
	virtual void onInitialize() override {
		LOG("Hello there!");
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
		if(ImGui::Button("Create Entity")) {
			scene.entities.push_back(std::make_shared<Entity>());
			scene.entities[scene.entities.size()-1]->name="New Entity";
		}
		ImGui::End();
		ImGui::Begin("Inspector");
		if(selection!=0) {
			ImGui::Text("Name: %s", scene.entities[selection-1]->name.c_str());
			ImGuiUtil::DragFloat3("Position",&scene.entities[selection-1]->transform.position);
			ImGuiUtil::DragFloat3("Rotation",&scene.entities[selection-1]->transform.rotation);
			ImGuiUtil::DragFloat3("Size",&scene.entities[selection-1]->transform.size);
			if(ImGui::Button("Remove")) {
				scene.removeAt(selection-1);
				selection=0;
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