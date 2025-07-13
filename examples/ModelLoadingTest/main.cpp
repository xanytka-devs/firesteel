#define FS_PRINT_DEBUG_MSGS
#include <../include/firesteel.hpp>
#include <../include/input/input.hpp>
using namespace Firesteel;

Shader shader;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;

class ModelLoadingTest : public Firesteel::App {
    virtual void onInitialize() override {
        shader=Shader("backpack\\.vs", "backpack\\.fs");
        entity.load("backpack\\backpack.fbx");
        camera.update();
    }
    virtual void onUpdate() override {
        //Process input.
        if(Keyboard::keyDown(KeyCode::KEY_1)) renderer.setDrawMode(Renderer::DM_FILL);
        if(Keyboard::keyDown(KeyCode::KEY_2)) renderer.setDrawMode(Renderer::DM_WIRE);
        entity.transform.rotation+=glm::vec3(0,-10*deltaTime*Input::getHorizontalAxis(),0);
        entity.transform.rotation+=glm::vec3(10*deltaTime*Input::getVerticalAxis(),0,0);
        //Get variables, needed for a draw call.
        glm::mat4 proj = camera.getProjection(), view = camera.getView();
        camera.aspect = window.aspect();
        //Draw the model.
        shader.enable();
        shader.setMat4("projection", proj);
        shader.setMat4("view", view);
        entity.draw(&shader);
    }
    virtual void onShutdown() override {
        entity.remove();
        shader.remove();
    }
};

int main() {
    return ModelLoadingTest{}.start();
}