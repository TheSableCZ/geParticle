#pragma once

#include <geParticle/ParticleFactory.h>
#include <geParticle/ParticleContainer.h>

namespace ge {
	namespace particle {

		class SoAParticleFactory : public ParticleFactory<int>
		{
		public:
			SoAParticleFactory() : ParticleFactory<int>() {}

			int createParticle(std::shared_ptr<ParticleContainer> particles) override {
				assert(particles->getType() == ParticleContainerType::SoA_CS);

				auto container = std::static_pointer_cast<StructureOfArraysContainer>(particles);

				int idx = container->createParticle();

				initParticle(idx, particles);

				return idx;
			}

		protected:
			virtual void initParticle(int idx, std::shared_ptr<ParticleContainer> particles) = 0;
		};
	}
}
