#pragma once

//#include <ParticleSystem/ParticleSystem.h>
#include <geCore/Updatable.h>
//#include <ParticleSystem/Particle.h>
#include <geParticle/ParticleFactory.h>
#include <memory>
//#include "ParticleFactory.h"
//#include "ParticleSystem.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
		//class ParticleSystem;
		//class ParticleFactory;

        class ParticleEmitter
        {
        public:
            virtual void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
