#ifndef FS_PARTICLES_H
#define FS_PARTICLES_H

#include "common.hpp"
#include "utils/utils.hpp"
#include "shader.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace Firesteel {

    struct Particle {
        glm::vec3 Position;
        glm::vec3 Velocity;
        glm::vec4 Color;
        float Life;

        Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
    };

    float RandomFloat() {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    class ParticleSystem {
    public:
        std::vector<Particle> particles;
        unsigned int VAO = 0, VBO = 0;
        unsigned int maxParticles;
        glm::vec3 emitterPosition;

        ParticleSystem(const glm::vec3& tPosition, const unsigned int& tMaxParticles = 1000, const std::string& tTexturePath = "",bool tInit = true) {
            emitterPosition = tPosition;
            maxParticles = tMaxParticles;
            if(tInit) init();
        }
        ~ParticleSystem() { remove(); }

        void init() {
            particles.resize(maxParticles);
            float particle_quad[] = {
                -0.05f, -0.05f, 0.0f,
                 0.05f, -0.05f, 0.0f,
                 0.05f,  0.05f, 0.0f,
                -0.05f,  0.05f, 0.0f
            };
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glBindVertexArray(0);
        }

        void update(const float& dt) {
            for (unsigned int i = 0; i < maxParticles; ++i) {
                Particle& p = particles[i];
                p.Life -= dt;
                if (p.Life > 0.0f) {
                    p.Position += p.Velocity * dt;
                    p.Color.a -= dt * 2.5f;
                } else respawn(p);
            }
        }

        void respawn(Particle& particle) const {
            particle.Position = emitterPosition;
            particle.Velocity = glm::vec3(10, 0, 0);
            particle.Color = glm::vec4(RandomFloat(), RandomFloat(), RandomFloat(), 1.0f);
            particle.Life = 1 + RandomFloat();
        }

        void draw(const Shader* shader) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            shader->enable();
            glBindVertexArray(VAO);
            for(size_t i = 0; i < particles.size(); i++) {
                if(particles[i].Life > 0.0f) {
                    shader->setVec3("offset", particles[i].Position - glm::vec3(particles[i].Life));
                    shader->setVec4("color", particles[i].Color);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                }
            }
            glBindVertexArray(0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }


        void remove() const {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    };

}

#endif // !FS_PARTICLES_H