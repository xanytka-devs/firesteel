#include <../include/firesteel.hpp>
using namespace Firesteel;

std::shared_ptr<Shader> shader;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;

class ModelLoading : public Firesteel::App {
    virtual void onInitialize() override {
        shader=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\shader.fs");
        entity.load("res\\ModelLoading\\backpack.obj");
        entity.setMaterialsShader(shader);
        camera.update();
    }
    virtual void onUpdate() override {
        //Get variables needed for a draw call.
        glm::mat4 proj = camera.getProjection(), view = camera.getView();
        camera.aspect = window.aspect();
        //Draw the model.
        shader->enable();
        shader->setMat4("projection", proj);
        shader->setMat4("view", view);
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
    }
};

int main() {
    return ModelLoading{}.start();
}