#pragma once

#include <geCore/Updatable.h>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
        class ParticleAffector
        {
        public:
			virtual void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
