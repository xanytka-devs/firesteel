/*
* This file is for various testing between major versions of Firesteel.
*/

#define FS_PRINT_DEBUG_MSGS
#include <firesteel/firesteel.hpp>
using namespace Firesteel;

class WindowApp : public Firesteel::App {
	void onInitialize() override {
		LOG("Hello World!");
		window.setVSync(true);
	}
};

int main() {
	WindowApp{}.start();
}