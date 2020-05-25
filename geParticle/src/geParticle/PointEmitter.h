#pragma once

#include <geParticle/BasicParticleEmitter.h>
#include <geParticle/SoAParticleFactory.h>

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
						Particle &p = factory ? std::static_pointer_cast<AoSParticleFactory>(factory)->createParticle(particles) : container->createParticle();

						p.pos = pos;
						p.livingFlag = true;
						// TODO: nekde v factory? minimalne parametrizovatelny
						p.life = core::time_unit(3.f);
					}
				}

				if (particles->getType() == ParticleContainer::SoA_CS) {
					auto container = std::static_pointer_cast<ComponentSystemContainer>(particles);

					for (int i = 0; i < newParticlesCount; i++) {
						int idx = factory ? std::static_pointer_cast<SoAParticleFactory>(factory)->createParticle(particles) : container->createParticle();

						auto &p = container->getComponent<Position>(idx);
						auto &l = container->getComponent<LifeData>(idx);

						p.position = pos;
						l.livingFlag = true;
						// TODO: nekde v factory?
						l.life = core::time_unit(3.f);
					}
				}
			}

		protected:
			glm::vec3 pos;
		};
	}
}
