#include <geParticle/ParticleContainerIterator.h>
#include <geParticle/ParticleContainer.h>

ge::particle::ParticleContainerType ge::particle::ParticleContainerIterator::getContainerType() const
{
	return container->getType();
}
