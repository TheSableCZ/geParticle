#include "ParticleEmitterBase.h"

void ge::particle::ParticleEmitterBase::emitParticles(core::time_unit dt,
                                                      std::shared_ptr<particle::ParticleContainer> particles)
{
	const int newParticlesCount = getNumOfParticlesToCreate(dt);

	if (newParticlesCount > 0)
	{
		auto unusedParticlesIterator = particles->getUnusedParticlesIterator();
		auto newRangeIterator = particles->createRangeIterator();

		for (int i = 0; i < newParticlesCount; i++)
		{
			auto newIdx = unusedParticlesIterator->getIndex();
			newRangeIterator->addIndex(newIdx);
			++(*unusedParticlesIterator);
		}
		for (auto& initiator : initiators)
			initiator->init(newRangeIterator);
	}
}
