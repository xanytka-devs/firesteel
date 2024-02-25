#include <memory>

#include <xengine/rendering/transform.hpp>
#include <xengine/app.hpp>

using namespace XEngine;

class Triangle : public Transform {
public:
    Triangle() : Transform(glm::vec3(0.f), glm::vec4(glm::vec3(0.f), 1.f), glm::vec3(1.f)) {}

    void initialize() {
        unsigned int vert_num = 3;
        float vertices[] = {
            //       POSITIONS                 NORMAL               UVs
                -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                 0.5f, -0.5f,  0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
                 0.0f,  0.5f,  0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f
        };
        std::vector<unsigned int> indicies(vert_num);
        for(unsigned int i = 0; i < vert_num; i++)
            indicies[i] = i;
        m_meshes.push_back(XEngine::Mesh(XEngine::Vertex::generate_list(vertices, vert_num), indicies));
        Transform::initialize();
    }

    void render(XEngine::Shader t_shader) {
        Transform::render(t_shader);
    }
};

Triangle triangle{};
Shader shader;

class TriangleApp : public App {
    virtual void initiazile() override {
        shader = Shader("res\\core.vert", "res\\core.frag");
        triangle.initialize();
    }

    virtual void update() override {
        shader.enable();
        shader.set_3_floats("aPos", triangle.position);
        triangle.render(shader);
    }

    virtual void on_shutdown() override {
        shader.remove();
    }
};

int main() {
    //Create instance of app.
    auto app = std::make_unique<TriangleApp>();
    int returnC = app->start(800, 600, "Triangle demo");
    return 0;
}