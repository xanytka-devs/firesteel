#define FS_PRINT_DEBUG_MSGS

#include <../include/firesteel.hpp>
Firesteel::Shader shader;
Firesteel::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Firesteel::Entity entity;

class ModelLoadingTest : public Firesteel::App {
    virtual void onInitialize() override {
        shader=Firesteel::Shader("backpack\\.vs", "backpack\\.fs");
        entity.load("backpack\\backpack.gltf");
        camera.update();
    }
    virtual void onUpdate() override {
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