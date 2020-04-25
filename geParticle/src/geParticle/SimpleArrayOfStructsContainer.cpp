/*#include <geParticle/SimpleArrayOfStructsContainer.h>

template <class T>
ge::particle::SimpleArrayOfStructsContainer<T>::SimpleArrayOfStructsContainer(int maxParticleCount)
	: maxParticles(maxParticleCount), lastUsedParticle(0)
{
	static_assert(std::is_base_of<Particle, T>::value);
	particles = new T[maxParticleCount];
}

template <class T>
ge::particle::SimpleArrayOfStructsContainer<T>::~SimpleArrayOfStructsContainer()
{
	delete[] particles;
}

template <class T>
ge::particle::Particle & ge::particle::SimpleArrayOfStructsContainer<T>::createParticle()
{
	return particles[findUnusedParticle()];
}

template <class T>
ge::particle::Particle & ge::particle::SimpleArrayOfStructsContainer<T>::getParticle(int idx)
{
	assert(idx < maxParticles);
	return particles[idx];
}

template <class T>
int ge::particle::SimpleArrayOfStructsContainer<T>::startIdx()
{
	return 0;
}

template <class T>
int ge::particle::SimpleArrayOfStructsContainer<T>::endIdx()
{
	return maxParticles - 1;
}

template <class T>
int ge::particle::SimpleArrayOfStructsContainer<T>::findUnusedParticle()
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
*/
