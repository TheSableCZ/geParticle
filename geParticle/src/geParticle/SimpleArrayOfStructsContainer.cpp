#include <geParticle/SimpleArrayOfStructsContainer.h>

ge::particle::SimpleArrayOfStructsContainer::SimpleArrayOfStructsContainer(int maxParticleCount)
	: maxParticles(maxParticleCount), lastUsedParticle(0)
{
	particles = new Particle[maxParticleCount];
}

ge::particle::SimpleArrayOfStructsContainer::~SimpleArrayOfStructsContainer()
{
	delete[] particles;
}

ge::particle::Particle & ge::particle::SimpleArrayOfStructsContainer::createParticle()
{
	return particles[findUnusedParticle()];
}

ge::particle::Particle & ge::particle::SimpleArrayOfStructsContainer::getParticle(int idx)
{
	assert(idx < maxParticles);
	return particles[idx];
}

int ge::particle::SimpleArrayOfStructsContainer::startIdx()
{
	return 0;
}

int ge::particle::SimpleArrayOfStructsContainer::endIdx()
{
	return maxParticles - 1;
}

int ge::particle::SimpleArrayOfStructsContainer::findUnusedParticle()
{
	for (int i = lastUsedParticle; i < maxParticles; i++) {
		if (!particles[i].livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (!particles[i].livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	// All particles are taken, override random particle
	return rand() % maxParticles;
}
