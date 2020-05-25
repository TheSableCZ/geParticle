#include <geParticle/ComponentSystemContainer.h>

ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount, bool registerStandardComponents)
	: maxParticles(maxParticleCount), StructureOfArraysContainer()
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
