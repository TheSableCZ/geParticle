#include <geParticle/ComponentSystemContainer.h>

ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount)
	: maxParticles(maxParticleCount),
	IndexBasedParticleContainer()
{}

std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ComponentSystemContainer::begin()
{
	return std::make_shared<iterator>(shared_from_this());
}

std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ComponentSystemContainer::end()
{
	return std::make_shared<iterator>(shared_from_this(), size());
}

std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ComponentSystemContainer::getUnusedParticlesIterator()
{
	if (!unusedParticlesIterator) {
		unusedParticlesIterator = std::make_shared<filter_iterator>(shared_from_this(), true);
	}
	return unusedParticlesIterator;
}

std::shared_ptr<ge::particle::RangeParticleContainerIterator> ge::particle::ComponentSystemContainer::createRangeIterator()
{
	return std::make_shared<range_iterator>(shared_from_this());
}
