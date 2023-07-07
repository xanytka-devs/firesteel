#include <iostream>
#include <memory>
#include <XEngineCore/App.hpp>

class NewApp : public XEngine::App {
	
	virtual void update() override { }

};

int main() {

	auto newApp = std::make_unique<NewApp>();

	int returnC = newApp->start(1024, 768, "Hello XEngine!");

	std::cin.get();
	return returnC;

}