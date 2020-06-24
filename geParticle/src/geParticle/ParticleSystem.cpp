#include <geParticle/ParticleSystem.h>

#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ParticleContainer.h>

ge::particle::ParticleSystem::ParticleSystem(std::shared_ptr<ParticleContainer> container)
{
	particleContainer = container;
}

void ge::particle::ParticleSystem::update(const ge::core::time_point &t) {
    core::time_unit dt = t-lastTime;
	lastTime = t;

	for (auto &pa : particleAffectors)
		pa->affect(dt, particleContainer);

    for (auto &pe : particleEmitters)
		pe->emitParticles(dt, particleContainer);
}

void ge::particle::ParticleSystem::addEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
	particleEmitters.push_back(emitter);
}

void ge::particle::ParticleSystem::addAffector(std::shared_ptr<ParticleAffector> affector)
{
	particleAffectors.push_back(affector);
}

std::shared_ptr<ge::particle::ParticleContainer> ge::particle::ParticleSystem::getParticleContainer()
{
	return particleContainer;
}
