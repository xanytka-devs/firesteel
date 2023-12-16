#include <iostream>
#include <memory>

#include <XEngine/App.hpp>

class EditorApp : public XEngine::App {

    virtual void initiate() override { }

	virtual void update() override { }

};

int main() {

    auto editor = std::make_unique<EditorApp>();

    int returnC = editor->start(800, 600, "Hello XEngine!");
    
    return 0;

}