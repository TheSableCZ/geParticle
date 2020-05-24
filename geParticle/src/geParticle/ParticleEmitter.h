#pragma once

#include <geCore/Updatable.h>
#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
        class ParticleEmitter
        {
        public:
            virtual void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
