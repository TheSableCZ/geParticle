#pragma once

#include <geCore/Updatable.h>
#include <geParticle/Particle.h>
#include <geParticle/ParticleContainer.h>
#include <geParticle/ParticleIterator.h>

#include <iostream>

namespace ge
{
    namespace particle
    {
        class ParticleAffector
        {
        public:
            //virtual void affect(core::time_unit dt, Particle &p) = 0;
			virtual void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
