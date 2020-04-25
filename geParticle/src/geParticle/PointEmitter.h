#pragma once

#include <geParticle/BasicParticleEmitter.h>

namespace ge {
	namespace particle {
		class PointEmitter : public BasicParticleEmitter
		{
		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos)
				: pos(pos), BasicParticleEmitter(particlesPerSecond) 
			{
			}

			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override {
				int newParticlesCount = getNumOfParticlesToCreate(dt);

				if (particles->getType() == ParticleContainer::AoS) {
					auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

					for (int i = 0; i < newParticlesCount; i++) {
						Particle &p = aosFactory ? aosFactory->createParticle(particles) : container->createParticle();

						p.pos = pos;
						p.livingFlag = true;
						p.life = core::time_unit(3.f);
					}
				}
			}

		protected:
			glm::vec3 pos;
		};
	}
}
