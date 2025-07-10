#define FS_PRINT_DEBUG_MSGS

#include <../include/firesteel.hpp>
#include <../include/input/keyboard.hpp>
Firesteel::Shader shader;
Firesteel::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Firesteel::Entity entity;

class ModelLoadingTest : public Firesteel::App {
    virtual void onInitialize() override {
        shader=Firesteel::Shader("backpack\\.vs", "backpack\\.fs");
        entity.load("backpack\\backpack.fbx");
        camera.update();
    }
    virtual void onUpdate() override {
        if(Firesteel::Keyboard::keyDown(Firesteel::KeyCode::KEY_1)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(Firesteel::Keyboard::keyDown(Firesteel::KeyCode::KEY_2)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(Firesteel::Keyboard::getKey(Firesteel::KeyCode::LEFT))  entity.transform.rotation-=glm::vec3(0,10*deltaTime,0);
        if(Firesteel::Keyboard::getKey(Firesteel::KeyCode::RIGHT)) entity.transform.rotation+=glm::vec3(0,10*deltaTime,0);
        glm::mat4 proj = camera.getProjection(), view = camera.getView();
        camera.aspect = window.aspect();
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