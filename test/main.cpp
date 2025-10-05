#include "../include/firesteel.hpp"
using namespace Firesteel;

class WindowApp : public Firesteel::App {
	virtual void onInitialize() override {
		LOG("Hello there!");
	}
};

int main() {
	WindowApp{}.start();
}