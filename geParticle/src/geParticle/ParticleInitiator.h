#pragma once

#include <memory>
#include <geParticle/ParticleContainerIterator.h>

namespace ge
{
    namespace particle
    {
		class ParticleInitiator {
		public:
			virtual void init(std::shared_ptr<RangeParticleContainerIterator> range) = 0;
		};
    }
}
