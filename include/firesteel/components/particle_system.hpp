#ifndef FS_COMP_PARTICLE_SYSTEM
#define FS_COMP_PARTICLE_SYSTEM
#include <firesteel/common.hpp>
#include <firesteel/component.hpp>
#include <firesteel/entity.hpp>
#include <firesteel/utils/delta_time.hpp>
#include <firesteel/utils/utils.hpp>

namespace Firesteel {
	struct Particle {
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec4 color;
		float life;

		Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
	};

	// @brief Particle system depended on Entity.
	// @warning This component is experimental. Use on your own risk.
	class ParticleSystem : public Component {
	public:
		ParticleSystem(Entity* tEntity, unsigned int tMaxParticles=100)
			: maxParticles(tMaxParticles), Component(tEntity) { }
		void onStart() override {
			mParticles.resize(maxParticles);
			float particle_quad[] = {
				-0.05f, -0.05f, 0.0f,
				 0.05f, -0.05f, 0.0f,
				 0.05f,  0.05f, 0.0f,
				-0.05f,  0.05f, 0.0f
			};
			glGenVertexArrays(1,&mVAO);
			glGenBuffers(1,&mVBO);
			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER,mVBO);
			glBufferData(GL_ARRAY_BUFFER,sizeof(particle_quad),particle_quad,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
			glBindVertexArray(0);
		}
		void onUpdate() override {
			float dt=DeltaTime::get();
			for(unsigned int i=0;i<maxParticles;i++) {
                Particle& p=mParticles[i];
                p.life-=dt;
                if(p.life>0.f) {
                    p.position+=p.velocity*dt;
                    p.color.a-=dt*2.5f;
                } else respawn(p);
            }
		}
		void onDraw() override {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			Shader* shader=mEntity->model.materials[0].getShader().get();
			shader->enable();
			glBindVertexArray(mVAO);
			for(size_t i=0;i<mParticles.size();i++) {
				if(mParticles[i].life<=0.0f) continue;
				shader->setVec3("offset", mParticles[i].position-glm::vec3(mParticles[i].life));
				shader->setVec4("color", mParticles[i].color);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			glBindVertexArray(0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		void onRemove() override {
			glDeleteVertexArrays(1, &mVAO);
			glDeleteBuffers(1, &mVBO);
		}

		void properties() override {
			PROPERTY(maxParticles);
		}
		const char* name() const override { return "fs.particle_system"; }
	private:
		std::vector<Particle> mParticles;
		unsigned int mVAO=0, mVBO=0;
		unsigned int maxParticles=100;

		void respawn(Particle& particle) const {
            particle.position=mEntity->transform.position;
            particle.velocity=glm::vec3(10,0,0);
            particle.color=glm::vec4(Random::get(1.f),Random::get(1.f),Random::get(1.f), 1.0f);
            particle.life=1+Random::get(1.f);
        }
	};
}
#endif // !FS_COMP_PARTICLE_SYSTEM