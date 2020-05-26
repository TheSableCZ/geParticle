#pragma once

#include <geParticle/ParticleFactory.h>
#include <geParticle/ParticleContainer.h>
#include <geParticle/Particle.h>

namespace ge {
	namespace particle {

		class AoSParticleFactory : public ParticleFactory<Particle&>
		{
		public:
			AoSParticleFactory() : ParticleFactory<Particle&>() {}

			Particle& createParticle(std::shared_ptr<ParticleContainer> particles) override {
				assert(particles->getType() == ParticleContainer::AoS);

				auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

				Particle &p = container->createParticle();

				initParticle(p);

				return p;
			}

		protected:
			virtual void initParticle(Particle &p) = 0;
		};
	}
}
