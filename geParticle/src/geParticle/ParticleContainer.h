#pragma once

#include <geParticle/Particle.h>
#include <geParticle/ParticleContainerType.h>
#include <geParticle/ParticleContainerIterator.h>

namespace ge {
	namespace particle {

		class ParticleContainer { 
		public:
			virtual ParticleContainerType getType() = 0;
			virtual unsigned int size() = 0;

			virtual std::shared_ptr<ParticleContainerIterator> begin() = 0;
			virtual std::shared_ptr<ParticleContainerIterator> end() = 0;
			virtual std::shared_ptr<ParticleContainerIterator> getUnusedParticlesIterator() = 0;
			virtual std::shared_ptr<RangeParticleContainerIterator> createRangeIterator() = 0;
		};

		class IndexBasedParticleContainer : public ParticleContainer {
		public:
			virtual int startIdx() = 0;
			virtual int endIdx() = 0;
		};

	}
}
