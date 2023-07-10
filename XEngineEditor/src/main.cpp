#include <iostream>
#include <memory>
#include <imgui.h>

#include <XEngine/App.hpp>

class EditorApp : public XEngine::App {
	
	virtual void update() override { }
	virtual void onUIDraw() override {

        //Draw ImGui.
        ImGui::Begin("Editor");
        ImGui::Text("General Testing");
        ImGui::ColorEdit3("Bg Color", App::bgColor);
        /*ImGui::Text("Matrix Operations");
        ImGui::SliderFloat3("Position", position, -1.f, 1.f);
        ImGui::SliderFloat3("Rotation", rotation, 0.f, 360.f);
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);*/
        ImGui::Text("Camera");
        ImGui::SliderFloat3("Camera Pos", camPosition, -10.f, 10.f);
        ImGui::SliderFloat3("Camera Rot", camRotation, 0.f, 360.f);
        ImGui::Checkbox("Prespective", &camIsPresp);
        ImGui::End();

	}

};

int main() {

	auto app = std::make_unique<EditorApp>();

	int returnC = app->start(1024, 768, "Hello XEngine!");

	return returnC;

}