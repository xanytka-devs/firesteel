#include "../include/firesteel.hpp"
using namespace Firesteel;

#include "../include/scene.hpp"

Scene scene;

class WindowApp : public Firesteel::App {
	virtual void onInitialize() override {
		LOG("Hello there!");
		scene.entities.push_back(std::make_shared<Entity>());
		scene.entities[0]->name="Test";
		scene.save("scene.json");
	}
};

int main() {
	WindowApp{}.start();
}