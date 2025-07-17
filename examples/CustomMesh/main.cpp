#define FS_PRINT_DEBUG_MSGS
#include <../include/firesteel.hpp>
using namespace Firesteel;

Shader shader;
Entity entity;

class TriangleTest : public Firesteel::App {
    virtual void onInitialize() override {
        shader=Shader("res\\CustomMesh\\shader.vs", "res\\CustomMesh\\shader.fs");
        //Setup mesh.
        const float setup[9]={
            -0.5f, -0.5f,  0.5f,
            0.5f,  -0.5f,  0.0f,
            0.0f,   0.5f,  0.0f
        };
        std::vector<Vertex> verticies(3);
        std::vector<unsigned int> indicies(3);
        for(unsigned int i=0;i<3;i++) {
            Vertex vert{};
            vert.position=glm::vec3(setup[i],setup[i+1],setup[i+2]);
            verticies[i]=vert;
            indicies[i]=i;
        }
        //Append mesh to the model.
        entity.addMesh(verticies,indicies,std::vector<Texture>{});
    }
    virtual void onUpdate() override {
        entity.draw(&shader);
    }
    virtual void onShutdown() override {
        entity.remove();
        shader.remove();
    }
};

int main() {
    return TriangleTest{}.start();
}