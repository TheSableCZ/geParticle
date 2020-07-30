#include <geParticle/ComponentSystemContainer.h>

ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount, bool fixedSize, unsigned int reallocBlockSize)
	: IndexBasedParticleContainer(), maxParticles(maxParticleCount), fixedSize(fixedSize), reallocBlockSize(reallocBlockSize)
{
	if (fixedSize)
	{
		assert(maxParticleCount >= 0 && "Bad fixedSize and maxParticleCount combination.");
		actSize = maxParticleCount;
	}
	else
	{
		actSize = reallocBlockSize;
	}
}

bool ge::particle::ComponentSystemContainer::resize(int reallocBlockCount)
{
	if (fixedSize) return false;

	const unsigned newSize = actSize + reallocBlockCount * reallocBlockSize;
	if (maxParticles > 0 && newSize > static_cast<unsigned>(maxParticles)) return false;

	for (auto &component : components)
		component.second->resize(newSize);
	
	actSize = newSize;
	return true;
}

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
		unusedParticlesIterator = std::make_shared<unused_particles_iterator>(shared_from_this(), true);
	}
	return unusedParticlesIterator;
}

std::shared_ptr<ge::particle::RangeParticleContainerIterator> ge::particle::ComponentSystemContainer::createRangeIterator()
{
	return std::make_shared<range_iterator>(shared_from_this());
}
