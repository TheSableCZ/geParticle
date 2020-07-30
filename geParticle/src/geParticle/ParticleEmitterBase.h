#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticle/Counter.h>
#include <geParticle/ParticleInitiator.h>

namespace ge
{
	namespace particle
	{
		class ParticleEmitterBase : public ParticleEmitter
		{
		public:
			ParticleEmitterBase(std::shared_ptr<Counter> counter)
				: counter(counter) {}

			void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

			void setCounter(std::shared_ptr<Counter> &counter) { this->counter = counter; }
			std::shared_ptr<Counter>& getRefCounter() { return counter; }

			std::vector<std::shared_ptr<ParticleInitiator>> initiators;

		protected:
			virtual unsigned int getNumOfParticlesToCreate(core::time_unit dt)
			{
				return counter->getNum(dt);
			}

			std::shared_ptr<Counter> counter;
		};
	}
}
