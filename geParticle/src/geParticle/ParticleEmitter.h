#pragma once

#include <geCore/Updatable.h>
#include <memory>
#include <geParticle/ParticleContainer.h>
#include <geParticle/ParticleInitiator.h>

namespace ge
{
    namespace particle
    {
        class ParticleEmitter
        {
        public:
            virtual void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };

		class ParticleEmitterBase : public ParticleEmitter
		{
		public:
			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override
			{
				int newParticlesCount = getNumOfParticlesToCreate(dt);
				auto unusedParticlesIterator = particles->getUnusedParticlesIterator();
				auto newRangeIterator = particles->createRangeIterator();

				for (int i = 0; i < newParticlesCount; i++) {
					auto newIdx = unusedParticlesIterator->getIndex();
					newRangeIterator->addIndex(newIdx);
					(*unusedParticlesIterator)++;
				}

				for (auto &initiator : initiators)
					initiator->init(newRangeIterator);
			}

			std::vector<std::shared_ptr<ParticleInitiator>> initiators;

		protected:
			virtual unsigned int getNumOfParticlesToCreate(core::time_unit dt) = 0;
		};
    }
}
