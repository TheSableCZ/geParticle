#pragma once

#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
		class IParticleFactory {};

		template <class T>
        class ParticleFactory : public IParticleFactory
        {
        public:
			virtual T createParticle(std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
