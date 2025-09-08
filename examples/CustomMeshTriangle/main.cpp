#include <../include/firesteel.hpp>
using namespace Firesteel;

Material material;
Entity entity;

class CustomMeshTriangle : public Firesteel::App {
    virtual void onInitialize() override {
        material.setShader("res\\CustomMesh\\shader.vs", "res\\CustomMesh\\shader.fs");
        //Setup mesh.
        const float setup[3][3]={
            { 1,     1,    0},
            { 0,    -1,    0},
            {-1,     1,    1}
        };
        std::vector<Vertex> verticies(3);
        std::vector<unsigned int> indicies{0,1,2};
        for(unsigned int i=0;i<3;i++) {
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
    return CustomMeshTriangle{}.start();
}