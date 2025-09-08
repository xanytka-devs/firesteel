#include <../include/firesteel.hpp>
using namespace Firesteel;

Material material;
Entity entity;

class CustomMeshQuad : public Firesteel::App {
    virtual void onInitialize() override {
        material.setShader("res\\CustomMesh\\shader.vs", "res\\CustomMesh\\shader.fs");
        //Setup mesh.
        const float setup[4][3]={
            {-1,    -1,    0},
            {-1,     1,    0.5f},
            { 1,    -1,    0.5f},
            { 1,     1,    1}
        };
        std::vector<Vertex> verticies(4);
        std::vector<unsigned int> indicies{0,3,2,1,3,0};
        for(unsigned int i=0;i<4;i++) {
            Vertex vert{};
            vert.position=glm::vec3(setup[i][0],setup[i][1],setup[i][2]);
            verticies[i]=vert;
        }
        //Append mesh to the model.
        entity.addMesh(verticies,indicies,&material);
        //Adjust model to be visible.
        entity.transform.rotation.x=180.f;
        entity.transform.size=glm::vec3(0.8f);
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
        material.remove();
    }
};

int main() {
    return CustomMeshQuad{}.start();
}