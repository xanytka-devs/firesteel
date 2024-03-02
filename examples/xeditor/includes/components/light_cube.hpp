#ifndef CUBE_H
#define CUBE_H

#include <xengine/rendering/transform.hpp>
#include <xengine/procedural/geometry.hpp>
#include "light_source.hpp"

class Cube : public XEngine::Transform {
private:
    void draw_light(Transform t, Shader t_shader) const {
        LightSource l = t.get_component<LightSource>();
        l.shader = &t_shader;
        l.light.position = t.position;
        l.light.color = color;
        l.update();
    }
public:
    Cube(glm::vec3 t_pos = glm::vec3(0.0f), glm::vec4 t_rot = glm::vec4(glm::vec3(0.f), 1.0f),
        glm::vec3 t_size = glm::vec3(1.0f), glm::vec4 t_color = glm::vec4(1.f))
        : XEngine::Transform(t_pos, t_rot, t_size, "Cube"), color(t_color) {}

    void initialize() {
        m_meshes.push_back(Geometry::Cube(glm::vec3(1.f)));
        get_component<LightSource>().color = color;
        Transform::initialize();
    }

    void render_light(XEngine::Shader t_shader) {
        draw_light(*this, t_shader);
        for(auto& inst : m_instances)
            draw_light(inst, t_shader);
    }

    void render(XEngine::Shader t_shader) {
        color = get_component<LightSource>().color;
        Transform::render(t_shader, false);
    }

    glm::vec4 color;
};

#endif // CUBE_H