#ifndef FS_COMP_PARTICLE_SYSTEM_H
#define FS_COMP_PARTICLE_SYSTEM_H
#include <firesteel/common.hpp>
#include <firesteel/component.hpp>
#include <firesteel/entity.hpp>
#include <firesteel/enviroment.hpp>
#include <firesteel/utils/delta_time.hpp>
#include <firesteel/utils/random.hpp>
#include <firesteel/utils/math.hpp>
#ifdef FS_COMPONENT_RENDERING
#include <firesteel/components/model.hpp>
#endif // FS_COMPONENT_RENDERING

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
	class ParticleSystemComponent : public Component {
	public:
		ParticleSystemComponent(std::shared_ptr<Entity> tEntity,
			const glm::vec4& tStartColor=glm::vec4(1), const glm::vec4& tEndColor=glm::vec4(0),
			const glm::vec3& tParticleStartVelocity=glm::vec3(0,-1,0), const float& tParticleLifetime=1, const float& tParticleLifetimeRandom=0.f,
			const uint& tMaxParticles=100)
			: maxParticles(tMaxParticles), Component(tEntity),
			startColor(tStartColor), endColor(tEndColor), particleStartVelocity(tParticleStartVelocity),
			particleLifetime(tParticleLifetime), particleLifetimeRandom(tParticleLifetimeRandom) {
				mParticles.resize(tMaxParticles);
			}
		void onStart() override {
			mParticles.resize(maxParticles);
			float particle_quad[] = {
				-0.05f, -0.05f, 0.0f,
				 0.05f, -0.05f, 0.0f,
				 0.05f,  0.05f, 0.0f,
				-0.05f,  0.05f, 0.0f
			};
			uint mVBO=0;
			glGenVertexArrays(1,&mVAO);
			glGenBuffers(1,&mVBO);
			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER,mVBO);
			glBufferData(GL_ARRAY_BUFFER,sizeof(particle_quad),particle_quad,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
			glBindVertexArray(0);
			glDeleteBuffers(1, &mVBO);
		}
		void onUpdate() override {
			float dt=DeltaTime::get();
			for(uint i=0;i<maxParticles;i++) {
                Particle& p=mParticles[i];
                p.life-=dt;
				float time=p.life/particleLifetime;
                if(p.life>0.f) {
                    p.position+=p.velocity*dt;
                    p.color=Math::lerp(startColor,endColor,time);
                } else respawn(p);
            }
		}
		void onDraw() override {
			//Enviroment::getRenderer()->setAlphaBlending(false);
			Shader* shader=
				mEntity->model.materials.size()!=0?
#ifdef FS_COMPONENT_RENDERING
				mEntity->getComponent<ModelComponent>()->model.materials[0].getShader().get():
#else
				mEntity->model.materials[0].getShader().get():
#endif
				Shader::getDefaultShader().get();
			shader->enable();
			glBindVertexArray(mVAO);
			for(size_t i=0;i<mParticles.size();i++) {
				if(mParticles[i].life<=0.0f) continue;
				shader->setVec3("offset", mParticles[i].position);
				shader->setVec4("color", mParticles[i].color);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			glBindVertexArray(0);
			//Enviroment::getRenderer()->setAlphaBlending(true);
		}
		void onRemove() override {
			glDeleteVertexArrays(1, &mVAO);
		}

		void properties() override {
			PROPERTY(maxParticles);
			PROPERTY(startColor);
			PROPERTY(endColor);
			PROPERTY(particleStartVelocity);
			PROPERTY(particleLifetime);
			PROPERTY(particleLifetimeRandom);
		}
		const char* name() const override { return "fs.particle_system"; }

		glm::vec4 startColor;
		glm::vec4 endColor;
		glm::vec3 particleStartVelocity;
		float particleLifetime;
		float particleLifetimeRandom;
	private:
		std::vector<Particle> mParticles;
		uint mVAO=0;
		uint maxParticles;

		void respawn(Particle& particle) const {
            particle.position=mEntity->transform.position;
            particle.velocity=particleStartVelocity;
            particle.color=startColor;
            particle.life=particleLifetime+(particleLifetimeRandom>=0.f?Random::get(particleLifetimeRandom):0);
        }
	};
}
#endif // !FS_COMP_PARTICLE_SYSTEM_H