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
	        virtual ~ParticleEmitter() = default;
	        virtual void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) = 0;
        };
    }
}
