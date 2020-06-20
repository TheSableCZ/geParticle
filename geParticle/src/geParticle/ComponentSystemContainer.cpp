#include <geParticle/ComponentSystemContainer.h>

ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount, bool registerStandardComponents)
	: maxParticles(maxParticleCount),
	StructureOfArraysContainer()
{
	if (registerStandardComponents) {
		registerComponent<LifeData>();
		registerComponent<Position>();
		registerComponent<Velocity>();
	}
}

int ge::particle::ComponentSystemContainer::createParticle()
{
	return findUnusedParticle();
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
		unusedParticlesIterator = std::make_shared<filter_iterator>(shared_from_this(), true);
	}
	return unusedParticlesIterator;
}

std::shared_ptr<ge::particle::RangeParticleContainerIterator> ge::particle::ComponentSystemContainer::createRangeIterator()
{
	return std::make_shared<range_iterator>(shared_from_this());
}

int ge::particle::ComponentSystemContainer::findUnusedParticle()
{
	for (int i = lastUsedParticle; i < maxParticles; i++) {
		if (!getComponent<LifeData>(i).livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (!getComponent<LifeData>(i).livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	// All particles are taken, override random particle
	return rand() % maxParticles;
}
