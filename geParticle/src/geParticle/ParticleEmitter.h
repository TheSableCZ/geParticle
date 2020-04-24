#pragma once

//#include <ParticleSystem/ParticleSystem.h>
#include <geCore/Updatable.h>
//#include <ParticleSystem/Particle.h>
#include <geParticle/ParticleFactory.h>
#include <memory>
//#include "ParticleFactory.h"
//#include "ParticleSystem.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
		//class ParticleSystem;
		//class ParticleFactory;

        class ParticleEmitter
        {
        public:
            virtual void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };

        class BasicParticleEmitter : public ParticleEmitter
        {
        public:
            //explicit BasicParticleEmitter(std::shared_ptr<ParticleSystem> ps);
            void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
            inline void setParticlesPerSecond(int numOfParticles) { particlesPerSecond = numOfParticles; }
            std::unique_ptr<ParticleFactory> particleFactory;

        protected:
            int getNumOfParticlesToCreate(core::time_unit dt);

			//std::shared_ptr<ParticleSystem> particleSystem;
            int particlesPerSecond = 0;
            double carryOver = .0f;
        };

		class PointEmitter : public BasicParticleEmitter
		{
		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos)
				: BasicParticleEmitter() {
				this->pos = pos;
				this->setParticlesPerSecond(particlesPerSecond);
			}
			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override {
				int newParticlesCount = getNumOfParticlesToCreate(dt);

				if (particles->getType() == ParticleContainer::AoS) {
					auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

					for (int i = 0; i < newParticlesCount; i++) {
						Particle &p = container->createParticle();

						p.pos = pos;
						p.livingFlag = true;
						p.life = core::time_unit(3.f);

						p.velocity = glm::vec3(0.5f, 0.5f, 0.f);
					}
				}
			}

		protected:
			glm::vec3 pos;
		};
    }
}
